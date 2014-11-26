
#include <cstdio>
#include <string.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

using namespace std;

#include <fcntl.h>

class Pin
{
  std::string id;
  std::string dir;
  
 public:
  Pin(std::string id, std::string dir) {
    this->id = id;
    d = NULL;
    gpio_path = "/sys/class/gpio/gpio";
    gpio_path.append(id);
    gpio_path.append("/value");
  }

  void init() {
    doUnexport();
    doExport();
    setDirection(dir);
  }

  void setDirection(std::string dir) {
    this->dir = dir;
    std::string path = "/sys/class/gpio/gpio";
    path.append(id);
    path.append("/direction");
    write_to(path, dir);
  }

  FILE *d ;
  std::string gpio_path;
  void set(std::string value) {
    if (d==NULL) {
      d = fopen(gpio_path.c_str(),"w");
      if (d==NULL) {
	printf("Fail opening file %s\n", gpio_path.c_str());
	exit(-1);
      }
    }
    fwrite(value.c_str(), value.length() * sizeof(char), 1, d);
    fflush(d);
  }
  
  void doExport() {
    write_to("/sys/class/gpio/export", id);
  }
  void doUnexport() {
    write_to("/sys/class/gpio/unexport", id);
  }


  char readPin() {
    int fd;
    fd = open(gpio_path.c_str(),O_RDONLY);
    if (fd == -1) {
      printf("Fail opening file %s\n", gpio_path.c_str());
      exit(-1);
    }
    char value;
    if(-1 == read(fd, &value, 1)) {
      printf("Fail reading file %s\n", gpio_path.c_str());
      exit(-1);
    }
    close(fd);
    return value;
  }

 private:
  void write_to(std::string file_path, std::string data) {
    FILE *f = fopen(file_path.c_str(),"w");
    if (f==NULL) {
      printf("Fail opening file %s\n", file_path.c_str());
      exit(-1);
    }
    //    if(
    fwrite(data.c_str(), data.length() * sizeof(char), 1, f);
    // != 0) {
    //      printf("Fail when writing '%s' to file %s\n", data.c_str(), file_path.c_str());
    //      exit(-1);
    //    }
    if(fclose(f)) {
      printf("Fail closing file %s\n", file_path.c_str());
      //      exit(-1);
    }
  }
};
/*
  GPIO mappings

  3.3v  5v 
  2     DNC
  3     
  4     
  x     
  x     
  x     
  x     
  x     
  x     
  x     
  x     
  GND  9


 */
#include <ctime>

class Clock : public Pin
{
 public:
  Clock(string id, string dir) : Pin(id, dir) {
  }
  void tick() {
      set("0");
      //      usleep(10);
      set("1");
      //      usleep(10);
  }
};

#include <map>



int64_t timespecDiff(struct timespec *timeA_p, struct timespec *timeB_p)
{
  return ((timeA_p->tv_sec * 1000000000) + timeA_p->tv_nsec) -
           ((timeB_p->tv_sec * 1000000000) + timeB_p->tv_nsec);
}

std::string getAddress(char *data) {
  switch(data[0]) {
  case 'A':
    return "0000";
    break;
  case 'B':
    return "X000";
    break;
  case 'C':
    return "0X00";
    break;
  default:
    cout << "Unsupported address\n";
    exit(-1);
  }
}

std::string getUnit(char *data) {
  switch(data[0]) {
  case '0':
    return "0000";
    break;
  case '1':
    return "X000";
    break;
  case '2':
    return "0X00";
    break;
  default:
    cout << "Unsupported unit" << endl;
    exit(-1);
  }
}

std::string getState(char *data) {

  if(strcmp(data,"on") == 0) {
    return "X";
  }
  else if(strcmp(data, "off") == 0) {
    return "0";
  }
  else {
    cout << "State should be 'on' or 'off'" << endl;
    exit(1);
  }
}

#include <sys/time.h>
int main(int argc, char* argv[]) 
{

  Pin tx = Pin("2", "in");
  tx.init();
  tx.setDirection("out");


  string on = "0000" "0000" "0XX" "X";
  string off = "0000" "0000" "0XX" "0";

  int repeats = 1;

  if(argc <4) {
    cout << "Missing arguments" << endl;
    cout << endl;
    cout << "usage: cmd A 0 on" << endl;
    exit(1);
  }
  else if(argc == 5) {
    repeats = int(argv[4][0] - '0');
  }
  for(int i=0; i < argc; i++) {
    cout << i << " " << argv[i] << endl;
  }
  string cmd = getAddress(argv[1]).append(getUnit(argv[2])).append("0XX").append(getState(argv[3]));

  cout << cmd << endl;

  int T = 330;
  int times = 3;
  struct timespec start, end;
  struct timespec t1, t2;

  for (int i = 0; i < repeats; i++) {
    clock_gettime(CLOCK_MONOTONIC, &start);

    for(int i=0; i<cmd.length(); i++)
    {
      if(cmd[i] == '1') {
	tx.set("1");
	usleep(T*times);
	tx.set("0");
	usleep(T);

	tx.set("1");
	usleep(T*times);
	tx.set("0");
	usleep(T);
      }
      else if(cmd[i] == 'X') {
	tx.set("1");
	usleep(T);
	tx.set("0");
	usleep(T*times);

	tx.set("1");
	usleep(T*times);
	tx.set("0");
	usleep(T);
      }
      else {
	tx.set("1");
	usleep(T);
	tx.set("0");
	usleep(T*times);

	tx.set("1");
	usleep(T);
	tx.set("0");
	usleep(T*times);
      }
    }
    tx.set("1");
    usleep(T);
    tx.set("0");
    usleep(T*32);
    clock_gettime(CLOCK_MONOTONIC, &end);

    int64_t timeElapsed = timespecDiff(&end, &start);
    int bit = timeElapsed/(12*2*4 + 33);
    cout << timeElapsed << "ns " << bit << endl;
    int delta = 400*1000 - bit;
    T += (delta)/1000;
  }

  tx.doUnexport();

  return 0;
}

