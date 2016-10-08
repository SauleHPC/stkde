#include <iostream>
#include <vector>
#include <string>
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

int main() {

  std::string csvline = "foo,\"bar,\"\"foo\",foo";
  std::vector<std::vector<std::string> > lines;

  ParseCSV (csvline, lines);

  for (auto a : lines) {
    for (auto b : a) {
      std::cout<<b<<"\t";
    }

    std::cout<<std::endl;
  }
}
