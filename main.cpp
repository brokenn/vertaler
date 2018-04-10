#include <cassert>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <streambuf>
#include <string>
#include <vector>
#include <map>

#include "token.h"
#include "scanner.h"

void run (const std::string &code)
{
  /*
   * Create scanner from code
   */
  Scanner scanner (code);

  /*
   * Scan tokens
   */
  const auto tokens = scanner.scan_tokens ();
  
  /*
   * Dump tokens for debugging
   */
  for (auto token : tokens)
  {
    std::cout << token << std::endl;
  }
}

/*
 * REPL prompt
 */
int run_prompt () {
  for (std::string line; std::getline (std::cin, line);)
  {
    run (line);
  }
  return EXIT_SUCCESS;
}

/*
 * Process file
 */
int run_file (const std::string &filename)
{
  std::ifstream is (filename);
  std::string text ((std::istreambuf_iterator<char>(is)),
                    std::istreambuf_iterator<char>());
  run (text);
  return EXIT_SUCCESS;
}

/*
 * Main
 */
int main (int argc, char **argv)
{
  if (argc == 1)
  {
    return run_prompt ();
  } 
  else if (argc == 2)
  {
    return run_file (argv[1]);
  }
  else
  {
    std::cerr << "Incorrect use" << std::endl;
    return EXIT_FAILURE;
  }
}
