#ifndef JSON_HPP
#define JSON_HPP

#include <vector>
#include <string>
#include <map>

namespace JSON {
	
	struct jsonType{
		std::string String;
		double Number;
		std::vector<jsonType> Array;
		std::map<std::string, jsonType> Object;
		char type;
	};
	
	struct JsonObject{std::map<std::string, jsonType> data;};
	
		class Parser {
		private:
		char ignore(char c){return (c=='{'||c=='}'||c=='[' ||c==']'||c==':'||c==','||c=='"')==0;}
		char isN(char c, char c2){return ((c>=48 && c<=57)||(c=='-' && c2>=48 && c2<=57));}
		
		jsonType parseNumber(std::string *src, int *start){
			std::string content;
			for(int i = (*start); this->ignore((*src)[i]) && i<(*src).length(); i++) content+=(*src)[i],(*start) = i;
			jsonType val = {Number:std::stod(content),type:'n'};
			return val;
		}
		
		jsonType parseString(std::string *src, int *start){
			std::string content;
			for(int i = (*start)+1; (*src)[i]!='"' && i<(*src).length(); i++){
				 if((*src)[i]=='\\') i+=1, content+="\\";
				 content+=(*src)[i];
				 (*start) = i;
			}
			jsonType val = {String:content, type:'s'};
			return val;
		}
		
		jsonType parseObject(std::string *src, int *start){
			std::string content;
			int cbc = 1;
			for(int i = (*start)+1; i<(*src).length(); i++){
				if((*src)[i]=='{') cbc+=1;
				if((*src)[i]=='}') cbc-=1;
				if(cbc==0) break;
				content+=(*src)[i];  
				(*start) = i;
			}
			jsonType val = {Object:this->Parse(&content), type:'o'}; 
			return val;
		}
		
		std::vector<jsonType> parseArray(std::string *src, int *start){
			std::vector<jsonType> data;
			int sb = 1;
			for(int i = (*start)+1; i<(*src).length(); i++){
				if((*src)[i]=='[') sb+=1, data.push_back({Array:this->parseArray(src,&i), type:'a'});
				if(sb<=0) break;
				if((*src)[i]==']') sb-=1;
				if((*src)[i]=='"') data.push_back(this->parseString(src,&i)), i+=1;
				if(isN((*src)[i],(*src)[i+1])) data.push_back(this->parseNumber(src,&i));
				if((*src)[i]=='{') data.push_back(this->parseObject(src,&i)), i+=1;
				(*start) = i;
			}
			return data;
		}
		
		public:
		std::map<std::string, jsonType> Parse(std::string *src){ 
			std::map<std::string, jsonType> data;
			std::string str, name;
			char c, stop = 0;
			int len = (*src).length();
			for(int i = 0; i<len; i++){
			  c = (*src)[i];
			  if(c<=32) continue;
		   	if(!stop && this->ignore(c)) str+=c;
		   	if(stop || (str[0]!='\0' && !this->ignore(c))){
		   	  if(c==':') stop = 1, name = str, str="";
		   	  if(stop && c=='"') data[name] = this->parseString(src,&i), stop = 0;
		   	  if(stop && c=='{') data[name] = this->parseObject(src,&i), stop = 0;
		   	  if(stop && c=='[') data[name] = {Array:this->parseArray(src,&i),type:'a'}, stop = 0;
		   	  if(stop && this->isN(c,(*src)[i+1])) data[name] = this->parseNumber(src,&i), stop = 0;
		    }
			}
			return data;
		}
		
	};
	
	class Stringifier {
		private:
		  std::string indent(std::string *s1){
		    std::string s2;
		    for(int i = 0; i<(*s1).length(); i++) {
					s2+=(*s1)[i];
					if((*s1)[i]=='\n') s2+="  ";
				}
				return s2;
		  }
      		
			std::string stringifyArray(std::vector<jsonType> *array){
			int l = 0;
		  std::string lb = "\n", s1, s2, res = "[\n", c = ",";
			for(auto elem : (*array)){
			  l+=1;
			  if(l==(*array).size()) c = " ";
			  if(lb[1]!=' ') res+= "  ";
				if(elem.type=='s') res+= '"'+elem.String+'"'+c+lb;
				if(elem.type=='n') res+= (std::to_string(elem.Number))+c+lb;
				if(elem.type=='o') {
				  std::string s = this->Stringify(&elem.Object);
				  res+= (this->indent(&s))+c+lb; 
				}
				if(elem.type=='a'){
					std::string s = this->stringifyArray(&elem.Array);
					res+= (this->indent(&s))+c+lb;
				}
			}
			res+= "]";
			return res;
		}
			
		public:
		std::string Stringify(std::map<std::string, jsonType> *json){
			std::string s1, s2, res = "{\n", lb = "\n", c=",";
			int l = 0;
			for(auto v : (*json)){
			  l+=1;
			  if(l==(*json).size()) c = " ";
			  if(lb[1]!=' ') res+= "  ";
				if((*json)[v.first].type=='s') res+= '"'+v.first+"\": "+'"'+((*json)[v.first].String)+'"'+c+lb;
				if((*json)[v.first].type=='n') res+= '"'+v.first+"\": "+(std::to_string((*json)[v.first].Number))+c+lb;
				if((*json)[v.first].type=='o'){
          std::string s = this->Stringify(&(*json)[v.first].Object);
					res+= '"'+v.first+"\": "+(this->indent(&s))+c+lb;
				}
				if((*json)[v.first].type=='a'){
					std::string s = this->stringifyArray(&(*json)[v.first].Array);
					res+= '"'+v.first+"\": "+(this->indent(&s))+c+lb;
				}
			}
			res+= "}";
			return res;
		}
	};

}
#endif 