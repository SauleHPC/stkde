#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
using namespace std;

//code from http://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
void ParseCSV(const string& csvSource, vector<vector<string> >& lines)
    {
       bool inQuote(false);
       bool newLine(false);
       string field;
       lines.clear();
       vector<string> line;

       string::const_iterator aChar = csvSource.begin();
       while (aChar != csvSource.end())
       {
          switch (*aChar)
          {
          case '"':
             newLine = false;
             inQuote = !inQuote;
             break;

          case ',':
             newLine = false;
             if (inQuote == true)
             {
                field += *aChar;
             }
             else
             {
                line.push_back(field);
                field.clear();
             }
             break;

          case '\n':
          case '\r':
             if (inQuote == true)
             {
                field += *aChar;
             }
             else
             {
                if (newLine == false)
                {
                   line.push_back(field);
                   lines.push_back(line);
                   field.clear();
                   line.clear();
                   newLine = true;
                }
             }
             break;

          default:
             newLine = false;
             field.push_back(*aChar);
             break;
          }

          aChar++;
       }

       if (field.size())
          line.push_back(field);

       if (line.size())
          lines.push_back(line);
    }

//returns whether a position could be obtained. 
//
//use the actual location if available,
//or pick a randomly uniform point in the box if a box is all we have.
bool longitude_of_record (const std::vector<std::string>& a, double* ret) {

    //longitude
    {
      std::stringstream ss;
      ss.str(a[51]);
      ss>>*ret;
      if (!ss) {//a[51] does not contain a number
	double box_long[4];
	{
	  std::stringstream ss2;
	  ss2.str(a[42]);
	  ss2>>box_long[0];
	  if (!ss2)
	    return false;
	}
	{
	  std::stringstream ss2;
	  ss2.str(a[44]);
	  ss2>>box_long[1];
	  if (!ss2)
	    return false;
	}
	{
	  std::stringstream ss2;
	  ss2.str(a[46]);
	  ss2>>box_long[2];
	  if (!ss2)
	    return false;
	}
	{
	  std::stringstream ss2;
	  ss2.str(a[48]);
	  ss2>>box_long[3];
	  if (!ss2)
	    return false;
	}
	*ret = box_long[0]+drand48()*(box_long[2]-box_long[0]);
	//	*ret = std::accumulate (box_long, box_long+4, 0.)/4.;
      }
    }

    return true;  
}

//returns whether a position could be obtained
//
//use the actual location if available,
//or pick a randomly uniform point in the box if a box is all we have.
bool latitude_of_record (const std::vector<std::string>& a, double* ret) {

    //longitude
    {
      std::stringstream ss;
      ss.str(a[52]);
      ss>>*ret;
      if (!ss) {//a[51] does not contain a number
	double box_lat[4];
	{
	  std::stringstream ss2;
	  ss2.str(a[43]);
	  ss2>>box_lat[0];
	  if (!ss2)
	    return false;
	}
	{
	  std::stringstream ss2;
	  ss2.str(a[45]);
	  ss2>>box_lat[1];
	  if (!ss2)
	    return false;
	}
	{
	  std::stringstream ss2;
	  ss2.str(a[47]);
	  ss2>>box_lat[2];
	  if (!ss2)
	    return false;
	}
	{
	  std::stringstream ss2;
	  ss2.str(a[49]);
	  ss2>>box_lat[3];
	  if (!ss2)
	    return false;
	}
	*ret = box_lat[0]+drand48()*(box_lat[1]-box_lat[0]);
	//*ret = std::accumulate (box_lat, box_lat+4, 0.)/4.;
	  //std::cerr<<"guessed"<<std::endl;
      }
    }

    return true;  
}

bool is_bisextile(int year) {
  if (year % 4 != 0) {return false;}
  else {
    if (year % 100 != 0) {return true;}
    else {
      if (year % 400 != 0) { return false;}
      else {
        return true;
      }
    }
  }
}

int YMD_to_daynumber_since_2000 (int year, int month, int day) {
  int ret = 0;
  for (int y = 2000; y< year; ++y)
    ret += is_bisextile(y)?366:365;

  for (int m = 1; m<month; ++m) {
    switch (m) {
    case 1: ret += 31; break;
    case 2: ret += 28; if (is_bisextile(year)) ret ++; break;
    case 3: ret += 31; break;
    case 4: ret += 30; break;
    case 5: ret += 31; break;
    case 6: ret += 30; break;
    case 7: ret += 31; break;
    case 8: ret += 31; break;
    case 9: ret += 30; break;
    case 10: ret += 31; break;
    case 11: ret += 30; break;
    case 12: ret += 31; break;
    }
  }
  
  ret += day;
  
  return ret;
}

//in format 2016-02-01T00:00:26.000Z 
bool str_to_time (const std::string& timestamp, double* out) {
  int month;
  int day;
  int year;
  char c;
  std::stringstream ss_time;
  ss_time.str(timestamp);
  ss_time >> year;
  ss_time >> c;
  ss_time >> month;
  ss_time >> c;
  ss_time >> day;

  if (ss_time.fail()) return false;

  *out = YMD_to_daynumber_since_2000 (year, month, day);  
  
  return true;
}

int main(int argc, char* argv[]) {

  std::ifstream in (argv[1]);

  int linenumber = 0;
  std::string s;

  {
    std::stringstream ss;
    ss << in.rdbuf();
    s = ss.str();
  }
  
  std::vector<std::vector<std::string> > lines;
  
  ParseCSV (s, lines);
  
  std::cout<<"X\tY\tT"<<std::endl;
  
  for (auto a : lines) {
    double time;

    if (! str_to_time(a[2], &time)) continue; 
    
    double longitude;
    if (!longitude_of_record (a, &longitude) ) continue;
    double latitude;
    if (!latitude_of_record (a, &latitude) ) continue;

    if (a[51].compare("") == 0)
      a[51] = "undef";
    if (a[52].compare("") == 0)
      a[52] = "undef";

    std::cout.precision(std::numeric_limits< double > ::max_digits10);
    //std::cout<<a[0]<<'\t'; //for debugging
    std::cout<<longitude<<'\t'<<latitude<<'\t'<<time<<std::endl;
	     // <<'\t'<<a[2]<<'\t'<<a[51]<<'\t'<<a[52]
	     // <<'\t'<<a[42]<<'\t'<<a[43]<<'\t'<<a[44]<<'\t'<<a[45]<<'\t'<<a[46]<<'\t'<<a[47]<<'\t'<<a[48]<<'\t'<<a[49]
	     // <<std::endl;
  }
}
