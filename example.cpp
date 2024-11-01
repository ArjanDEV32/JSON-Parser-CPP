#include "JSON.hpp"
#include <iostream>


int main(){
  JSON::Parser p;
  JSON::Stringifier s;
  JSON::JsonObject ds;
  std::string json =R"(
    {
      "ID": "SGML",
      "SortAs": "SGML",
      "GlossTerm": "Standard Generalized Markup Language",
      "Acronym": "SGML",
      "Abbrev": 88791986,
      "data": "Click Here",
      "size": 36,
      "style": "bold",
      "name": "text1",
      "hOffset": 250,
      "vOffset": 100,
      "a":{"a":"asdwd", "b": 23, "c":{"d":55, "h":55, "f":55}},
       "b":[12, {"a":34}, "Asdw", [23,45]]
    }
  )";
  ds.data = p.Parse(&json);
  
  //get types 
  std::cout << ds.data["b"].type<<std::endl;
  std::cout << ds.data["a"].type<<std::endl;
  std::cout << ds.data["data"].type<<std::endl;
  std::cout << ds.data["size"].type<<std::endl;

  //get values
  std::cout << ds.data["data"].String<<std::endl;
  std::cout << ds.data["size"].Number<<std::endl;

  std::cout << ds.data["a"].Object["a"].String<<std::endl;
  std::cout << ds.data["a"].Object["c"].Object["d"].Number<<std::endl;

  std::cout << ds.data["b"].Array[0].Number<<std::endl;
  std::cout << ds.data["b"].Array[1].Object["a"].Number<<std::endl;
  std::cout << ds.data["b"].Array[3].Array[0].Number<<std::endl;

  std::cout<<s.Stringify(&ds.data)<<std::endl;
  
  return 0;
}