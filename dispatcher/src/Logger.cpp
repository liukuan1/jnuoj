/* 
 * File:   Logger.cpp
 * Author: 51isoft
 * 
 * Created on 2014年1月18日, 下午12:51
 */

#include "Logger.h"
#include "GlobalHelpers.h"

Logger* Logger::instance = new Logger;
pthread_mutex_t Logger::log_mutex = PTHREAD_MUTEX_INITIALIZER;
const string Logger::LOG_DIRECTORY = "log/";

Logger::Logger() {
  //ctor
}

Logger * Logger::Getinstance() {
  return instance;
}

Logger::~Logger() {
  //dtor
}

void Logger::log(const char* msg) {
  log((string) msg);
}

void Logger::log(char* msg) {
  log((string) msg);
}

void Logger::log(string msg) {
  string filename = LOG_DIRECTORY + name_prefix + currentDate() + ".log";
  vector <string> messages = split(msg, '\n');

  pthread_mutex_lock(&log_mutex);

  FILE * fp = fopen(filename.c_str(), "a");
  while (fp == NULL) {
    usleep(50000);
    fp = fopen(filename.c_str(), "a");
  }

  string id = identifier.find(pthread_self()) == identifier.end() ?
      "Main" : identifier[pthread_self()];
  for (vector <string>::iterator it = messages.begin(); it != messages.end();
      ++it) {
    fprintf(fp, "%s %s[%llu]: %s\n", currentDateTime().c_str(), id.c_str(),
            (unsigned long long) pthread_self() % 10000, it -> c_str());
  }

  fclose(fp);

  pthread_mutex_unlock(&log_mutex);
}

/**
 * Add thread_id to name conversion
 * @param pt    Thread_id
 * @param id    Name
 */
void Logger::addIdentifier(pthread_t pt, string id) {
  identifier[pt] = id;
}

/**
 * Erase a thread_id conversion
 * @param pt    Thread_id
 */
void Logger::eraseIdentifier(pthread_t pt) {
  identifier.erase(pt);
}