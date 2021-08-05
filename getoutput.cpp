#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <string>
#include <iostream>
#include <iterator>

using namespace std;

int main(int argc, const char* argv[]) {
  int fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
  close(fd);
  for (int i = 0; i < 31; ++i) {
    string filename = "dst/target.py";
    if (i > 0) {
      filename = "dst/target-" + to_string(i) + ".py";
    }
    string ans = "answer.txt";
    int fd = open(ans.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU); // create output file
    int save_stdout = dup(STDOUT_FILENO); // save stdout
    close(STDOUT_FILENO); // redirect the standard output to a file (printf->file)
    dup(fd); // redirecting
    auto pid = fork(); // compile the code in a child process
    if (pid == 0) {
      execlp("python3", "python3", filename.c_str(), NULL);
      // execlp("./print_out.out", "./print_out.out", filename.c_str(), NULL);
      printf("some error occur: %s\n", filename.c_str());
      exit(0);
    }
    while (waitpid(pid, NULL, WNOHANG) == 0); // wait for the child ending
    close(fd); // finish processing
    close(STDOUT_FILENO); // finish processing
    dup2(save_stdout, STDOUT_FILENO); // re-open the stdout
    close(save_stdout);
    /*
      read output from file
    */
    ifstream ifs(ans);
    string ss;
    ifs >> ss;
    cout << "i: " << i << " " << ss << endl;
    ofstream ofs("out.txt", ofstream::app);
    ofs << "-------\n" << ss << "\n" << "########" << endl;
    ofs.close();
    ifs.close();
  }
  return 0;
}