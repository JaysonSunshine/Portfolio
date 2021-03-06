/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <climits>
#include <algorithm>
#include "Bruinbase.h"
#include "SqlEngine.h"
#include "RecordFile.h"
#include "BTreeIndex.h"

using namespace std;

// external functions and variables for load file and sql command parsing 
extern FILE* sqlin;
int sqlparse(void);

typedef struct {
  RecordId  rid;  // page number. the first page is 0
  int     key;  // slot number. the first slot is 0
} SearchData;

typedef struct {
  int  key;  // page number. the first page is 0
  string    value;  // slot number. the first slot is 0
  RecordId rid;
} Output;


bool compareByPid(const SearchData &a, const SearchData &b)
{
    return a.rid.pid < b.rid.pid;
    //a.word.size() < b.word.size();
}

bool compareByKey(const Output &a, const Output &b)
{
    return a.key < b.key;
    //a.word.size() < b.word.size();
}

bool compareByValue(const Output &a, const Output &b)
{
    return a.value < b.value;
    //return strcmp(a.value.c_str(), b.value.c_str());
    //a.word.size() < b.word.size();
}

RC SqlEngine::run(FILE* commandline)
{
  fprintf(stdout, "Bruinbase> ");

  // set the command line input and start parsing user input
  sqlin = commandline;
  sqlparse();  // sqlparse() is defined in SqlParser.tab.c generated from
               // SqlParser.y by bison (bison is GNU equivalent of yacc)

  return 0;
}

RC SqlEngine::select(int attr, const string& table, const vector<SelCond>& cond)
{
  RecordFile rf;   // RecordFile containing the table
  BTreeIndex index_tree;
  RecordId   rid;  // record cursor for table scanning
  ofstream outFile;
  //outFile.open("output", ios_base::app);

  RC     rc;
  int    key;     
  string value;
  int    count;
  int    diff;
  int difference;
  bool    equal = false;
  bool   any_NE = false;

  int locate_value;
  std::vector<Output> OutputSet;
  Output entry;
  bool value_checked = false;
  std::vector<SearchData> RecordSet;
  std::vector<int> notEqual;

  // open the table file
  if ((rc = rf.open(table + ".tbl", 'r')) < 0) {
    fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
    return rc;
  }

  //does the table have an index?
  //does cond.attr = 1 for any of the elements?
  bool index = false;
  for(int i = 0; i < cond.size(); i++){
    if(cond[i].attr == 1){
      index = true;
    }
  }
  rc = index_tree.open(table + ".index", 'r');
  if(rc == 0 && attr == 4 && cond.size() == 0){
    int total_keys = index_tree.getTotalKeys();
    fprintf(stdout, "%d\n",  total_keys);
      return 0; //more error checking here?
    }
  if(rc == 0 && index) {
    count = 0;
    IndexCursor cursor;
    //for single entry look up this, for multiple entries must determine minimum entry
    int minimum = 0;
    int maximum = INT_MAX;
    for(int i = 0; i < cond.size(); i++){
      if(cond[i].attr == 1){
        switch(cond[i].comp){
          case SelCond::EQ:
            if (minimum < atoi(cond[i].value)) minimum = atoi(cond[i].value);
            if (maximum > atoi(cond[i].value)) maximum = atoi(cond[i].value);
            if (maximum < atoi(cond[i].value)) goto end;
            if (minimum > atoi(cond[i].value)) goto end;
            break;
                case SelCond::GT:
            if(minimum < atoi(cond[i].value)) minimum = atoi(cond[i].value) + 1;
            break;
                case SelCond::LT:
            if(maximum > atoi(cond[i].value)) maximum = atoi(cond[i].value) - 1;
            break;
                case SelCond::GE:
            if(minimum < atoi(cond[i].value)) minimum = atoi(cond[i].value);
            break;
                case SelCond::LE:
            if(maximum > atoi(cond[i].value)) maximum = atoi(cond[i].value);
            break;
               case SelCond::NE:
            if(cond.size() == 1) goto no_index;
            if(!any_NE){
              for(int i = 0; i < cond.size(); i++){
                if(cond[i].comp != SelCond::NE){
                    any_NE = true;
                }
              }
            }
            if(any_NE){
              notEqual.push_back(atoi(cond[i].value));
            }
            else{
              goto no_index;
            }
            //if(maximum > atoi(cond[i].value)) maximum = atoi(cond[i].value);
            break;
        }
    }
  }
  difference = maximum - minimum;
  if(minimum <= maximum){
        locate_value = index_tree.locate(minimum, (IndexCursor&)cursor);
        //index_tree.readForward((IndexCursor&)cursor, (int&)key, (RecordId&)rid);
        //RecordSet.push_back(rid);
  }
  
  while(minimum <= maximum){ //what about end of last node?
        //what should we return for each tuple that fits criteria
    
    index_tree.readForward((IndexCursor&)cursor, (int&)key, (RecordId&)rid);
    if(key <= maximum){
      for(int i = 0; i < notEqual.size(); i++){
        if(key == notEqual[i]){
          equal = true;
        }
      }
      if(!equal){
        SearchData node;
        node.rid = rid;
        node.key = key;
        RecordSet.push_back(node);
      }
    }
    if(difference == 0){
      maximum = 0;
      if(locate_value != 1){
        goto count;
      }
    }
    minimum = key;
    if(cursor.pid == -1){
      maximum = 0;
    }
    equal = false;
  }
  sort(RecordSet.begin(), RecordSet.end(), compareByPid);
  for(int j = 0; j < RecordSet.size(); j++){
    for (unsigned i = 0; i < cond.size(); i++) {
        // compute the difference between the tuple value and the condition value
        if(cond[i].attr == 2) {
          rf.read(RecordSet[j].rid, key, value);
          value_checked = true;
          diff = strcmp(value.c_str(), cond[i].value);
              // skip the tuple if any value condition is not met
              switch (cond[i].comp) {
              case SelCond::EQ:
          if (diff != 0) goto tuple;
          break;
              case SelCond::NE:
          if (diff == 0) goto tuple;
          break;
              case SelCond::GT:
          if (diff <= 0) goto tuple;
          break;
              case SelCond::LT:
          if (diff >= 0) goto tuple;
          break;
              case SelCond::GE:
          if (diff < 0) goto tuple;
          break;
              case SelCond::LE:
          if (diff > 0) goto tuple;
          break;
              }
      }
    }
    if(value_checked){
      entry.key = key;
        entry.value = value;
        entry.rid = RecordSet[j].rid;
      OutputSet.push_back(entry);
    }
    else{
      entry.key = RecordSet[j].key;
      entry.rid = RecordSet[j].rid;
      OutputSet.push_back(entry);
      value_checked = false;
    }
    count++;
    tuple:
    //index_tree.readForward((IndexCursor&)cursor, (int&)key, (RecordId&)rid);
    //minimum = key;
    
      //printf("Key = %d\nValue = %s\n", key, value.c_str());
    ;
  }
  switch (attr) {
    case 1:  // SELECT key
      //outFile << key << "\n";
      sort(OutputSet.begin(), OutputSet.end(), compareByKey);
       for(int i = 0; i < OutputSet.size(); i++){
      fprintf(stdout, "%d\n", OutputSet[i].key);
    }
      break;
    case 2:  // SELECT value
      if(!value_checked){
        for(int i = 0; i < OutputSet.size(); i++){
          rf.read(OutputSet[i].rid, key, value);
          OutputSet[i].value = value;
        //fprintf(stdout, "%s\n", value.c_str());
      }
    }
      sort(OutputSet.begin(), OutputSet.end(), compareByValue);
      for(int i = 0; i < OutputSet.size(); i++){
        fprintf(stdout, "%s\n", (OutputSet[i].value).c_str());
      }
      break;
    case 3:  // SELECT *
      sort(OutputSet.begin(), OutputSet.end(), compareByKey);
      for(int i = 0; i < OutputSet.size(); i++){
        if(!value_checked){
          rf.read(OutputSet[i].rid, key, value);
           fprintf(stdout, "%d '%s'\n", OutputSet[i].key, value.c_str());
        }
        else{
          fprintf(stdout, "%d '%s'\n", OutputSet[i].key, (OutputSet[i].value).c_str());
        } 
      }
    break;
  }
  // print matching tuple count if "select count(*)"
    if (attr == 4) {
      //outFile << count << "\n";
      fprintf(stdout, "%d\n", count);
    }
  }
  else{
  // scan the table file from the beginning
    no_index:
    rid.pid = rid.sid = 0;
    count = 0;
    while (rid < rf.endRid()) {
      // read the tuple
      if ((rc = rf.read(rid, key, value)) < 0) {
        fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
        goto exit_select;
      }

      // check the conditions on the tuple
      for (unsigned i = 0; i < cond.size(); i++) {
        // compute the difference between the tuple value and the condition value
        switch (cond[i].attr) {
        case 1:
  	diff = key - atoi(cond[i].value);
  	break;
        case 2:
  	diff = strcmp(value.c_str(), cond[i].value);
  	break;
        }

        // skip the tuple if any condition is not met
        switch (cond[i].comp) {
        case SelCond::EQ:
  	if (diff != 0) goto next_tuple;
  	break;
        case SelCond::NE:
  	if (diff == 0) goto next_tuple;
  	break;
        case SelCond::GT:
  	if (diff <= 0) goto next_tuple;
  	break;
        case SelCond::LT:
  	if (diff >= 0) goto next_tuple;
  	break;
        case SelCond::GE:
  	if (diff < 0) goto next_tuple;
  	break;
        case SelCond::LE:
  	if (diff > 0) goto next_tuple;
  	break;
        }
      }

      // the condition is met for the tuple. 
      // increase matching tuple counter
      count++;
      // print the tuple 
      switch (attr) {
      case 1:  // SELECT key
        fprintf(stdout, "%d\n", key);
        break;
      case 2:  // SELECT value
        fprintf(stdout, "%s\n", value.c_str());
        break;
      case 3:  // SELECT *
        fprintf(stdout, "%d '%s'\n", key, value.c_str());
        break;
      }

      // move to the next tuple
      next_tuple:
      ++rid;
    }
  // print matching tuple count if "select count(*)"
    count:
    if (attr == 4) {
      fprintf(stdout, "%d\n", count);
    }
  }
  end:
  rc = 0;

  // close the table file and return
  exit_select:
  rf.close();
  return rc;
}

RC SqlEngine::load(const string& table, const string& loadfile, bool index)
{
  ifstream inFile;
  string line;
  int key;
  string value;
  RecordId rid;
  RecordFile tablefile;
  inFile.open(loadfile.c_str());
  string tablename = (string) table + ".tbl";
  tablefile.open(tablename, 'w');
  if(!index){
    while(getline(inFile, line)){
      parseLoadLine((const string&) line, (int&) key, (string&) value);
      tablefile.append(key, (const string&) value, (RecordId&) rid);
    }
  }
  else{
    BTreeIndex indexfile;
    string indexname = (string) table + ".index";
    indexfile.open(indexname, 'w');
    while(getline(inFile, line)){
      parseLoadLine((const string&) line, (int&) key, (string&) value);
      tablefile.append(key, (const string&) value, (RecordId&) rid);
      //printf("Page = %d", rid.pid);
      indexfile.insert(key, (const RecordId&) rid);
    }
    indexfile.close();
  }
  tablefile.close();
  return 0;
}

RC SqlEngine::parseLoadLine(const string& line, int& key, string& value)
{
    const char *s;
    char        c;
    string::size_type loc;
    
    // ignore beginning white spaces
    c = *(s = line.c_str());
    while (c == ' ' || c == '\t') { c = *++s; }

    // get the integer key value
    key = atoi(s);

    // look for comma
    s = strchr(s, ',');
    if (s == NULL) { return RC_INVALID_FILE_FORMAT; }

    // ignore white spaces
    do { c = *++s; } while (c == ' ' || c == '\t');
    
    // if there is nothing left, set the value to empty string
    if (c == 0) { 
        value.erase();
        return 0;
    }

    // is the value field delimited by ' or "?
    if (c == '\'' || c == '"') {
        s++;
    } else {
        c = '\n';
    }

    // get the value string
    value.assign(s);
    loc = value.find(c, 0);
    if (loc != string::npos) { value.erase(loc); }

    return 0;
}
