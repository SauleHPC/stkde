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

//the flu dataset is in tab separated format
//
//date strain lat long
//
//date is in MM/DD/YYYY format
//strain is a string
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

      std::string s_time, s_strain, s_lat, s_long;
      getline(ss_line, s_time, '\t');
      getline(ss_line, s_strain, '\t');
      getline(ss_line, s_lat, '\t');
      getline(ss_line, s_long, '\t');

      if (s_time.compare("-N/A-") == 0)
	continue;
      if (s_long.compare("-N/A-") == 0)
	continue;
      if (s_lat.compare("-N/A-") == 0)
	continue;
      
      if (! ss_line.fail()) {
	//parse time
	{
	  int month;
	  int day;
	  int year;
	  char c;
	  std::stringstream ss_time;
	  ss_time.str(s_time);
	  ss_time >> month;
	  ss_time >> c;
	  ss_time >> day;
	  ss_time >> c;
	  ss_time >> year;
	  
	  if (! ss_time.fail() ) {
	    
	    //std::cout<<year<<"+"<<month<<"+"<<day;
	    time = YMD_to_daynumber_since_2000 (year, month, day); //a bit dirty
	    
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
