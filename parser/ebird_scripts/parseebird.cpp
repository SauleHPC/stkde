#include <iostream>
#include <sstream>

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

int YMD_to_daynumber_since_1800 (int year, int month, int day) {
  int ret = 0;
  for (int y = 1800; y< year; ++y)
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

//the ebird dataset after python script is in tab separated format
//
//lat long date time
//
//date is in YYYY-MM-DD format
//time is in HH:MM:SS format
//lat and long are in degrees

int main () {

  std::cout<<"X\tY\tT"<<std::endl;

  //skip first line
  {
    std::string s;
    getline (std::cin, s);
  }
  
  while (std::cin) {
    int time;
    double latitude;
    double longitude;

    std::string s;
    getline (std::cin, s);
    
    if (std::cin) {
      std::stringstream ss_line;
      ss_line.str(s);

      std::string s_time, s_date, s_lat, s_long;
      getline(ss_line, s_lat, '\t');
      getline(ss_line, s_long, '\t');
      getline(ss_line, s_date, '\t');
      getline(ss_line, s_time, '\t');
      
      if (! ss_line.fail()) {
	//parse time
	{
	  int month;
	  int day;
	  int year;
	  char c;
	  std::stringstream ss_date;
	  ss_date.str(s_date);
	  ss_date >> year;
	  ss_date >> c;
	  ss_date >> month;
	  ss_date >> c;
	  ss_date >> day;
	  
	  if (! ss_time.fail() ) {
	    
	    //std::cout<<year<<"+"<<month<<"+"<<day;
	    time = YMD_to_daynumber_since_1800 (year, month, day); //a bit dirty
	    
	    //check for multiple latitudes (and skip if that happens)
	    //or -N/A-
	    {
	      bool skip = false;
	      for (auto c : s_long) {
		if (c == ',') {
		  skip = true;
		  break;
		}
	      }

	      if (!skip)
		std::cout <<s_lat<<"\t"<<s_long<<"\t"<< time <<std::endl;	      
	    }
	  }
	}
      }
    }
  }
}
