#include <iostream>
#include <fstream>
#include <omp.h>

using namespace std;

int main(int argc, char const *argv[]) 
{
  std::ifstream myFile;
	std::string line;
	std::string allDNA = "";


  // Open file in reading mode
	myFile.open ("dna2.txt", std::ifstream::in);

  int countA = 0;
  int countC = 0;
  int countG = 0;
  int countT = 0;
  int countN = 0;
  double totalNucleotides;
  double t1;
  double t2;
  double t3;

	if (myFile.is_open())
	{
    // Read all the lines of the file into the string allDNA
    // Warning: This is not recommended for very big files
  	while ( getline (myFile, line) )
  		allDNA += line;
  
    // Close the file
  	myFile.close();

    // Total number of character (after removing the newline character '\n')
    totalNucleotides = allDNA.length();

    /* Code the sequential part here (remember to time it */
    t1 = omp_get_wtime();
    for(int i = 0; i < totalNucleotides; i++){
      if(allDNA.at(i) == 'A'){
        countA++;
      }else if(allDNA.at(i) == 'C'){
        countC++;
      }else if(allDNA.at(i) == 'G'){
        countG++;
      }else if(allDNA.at(i) == 'T'){
        countT++;
      }else if(allDNA.at(i) == 'N'){
        countN++;
      }
    }
    t1 = omp_get_wtime() - t1;

    // /* Print out number of nucleotides*/
    // cout << "Total amount of nucleotides: " << totalNucleotides << endl;
    // cout << "A: " << countA << endl;
    // cout << "C: " << countC << endl;
    // cout << "G: " << countG << endl;
    // cout << "T: " << countT << endl;
    // cout << "N: " << countN << endl;

    /* Reset counts */
    countA = 0;
    countC = 0;
    countG = 0;
    countT = 0;
    countN = 0;

    /* Start timing here */
    t2 = omp_get_wtime();
    #pragma omp parallel num_threads(5)
    {
      #pragma omp sections
      {
        // Counts all nucleotides A
        #pragma omp section
        {
          // #pragma omp for schedule(static, 1)
          for(int i = 0; i < totalNucleotides; i++){
            if(allDNA.at(i) == 'A'){
              countA++;
            }
          }
        }

        // Counts all nucleotides C
        #pragma omp section
        {
          for(int i = 0; i < totalNucleotides; i++){
            if(allDNA.at(i) == 'C'){
              countC++;
            }
          }
        }

        // Counts all nucleotides G
        #pragma omp section
        {
          for(int i = 0; i < totalNucleotides; i++){
            if(allDNA.at(i) == 'G'){
              countG++;
            }
          }
        }

        // Counts all nucleotides T
        #pragma omp section
        {
          for(int i = 0; i < totalNucleotides; i++){
            if(allDNA.at(i) == 'T'){
              countT++;
            }
          }
        }

        // Counts all non-nucleotides N
        #pragma omp section
        {
          for(int i = 0; i < totalNucleotides; i++){
            if(allDNA.at(i) == 'N'){
              countN++;
            }
          }
        }
      }
    }
    t2 = omp_get_wtime() - t2;
    /* End timing here */

    int countsAll[5] = {};
    t3 = omp_get_wtime();
    #pragma omp parallel num_threads(10) reduction(+:countsAll)
    {
      for(int i = 0; i < totalNucleotides; i++){
          switch(allDNA.at(i)){
            case 'A':
              countsAll[0]++;
              break;
            case 'C':
              countsAll[1]++;
              break;
            case 'G':
              countsAll[2]++;
              break;
            case 'T':
              countsAll[3]++;
              break;
            case 'N':
              countsAll[4]++;
              break;
          }
      }
    }
    t3 = omp_get_wtime() - t3;
	}

  /* Print your results here */
  cout << "Total amount of nucleotides: " << totalNucleotides << endl;
  cout << "A: " << countA << "\t(" << (countA/totalNucleotides) * 100 << "%)" << endl;
  cout << "C: " << countC << "\t(" << (countC/totalNucleotides) * 100 << "%)" << endl;
  cout << "G: " << countG << "\t(" << (countG/totalNucleotides) * 100 << "%)" << endl;
  cout << "T: " << countT << "\t(" << (countT/totalNucleotides) * 100 << "%)" << endl;
  cout << "N: " << countN << "\t(" << (countN/totalNucleotides) * 100 << "%)" << endl;
  cout << endl;
  cout << "Sectional Time: " << t2 << "secs." << endl;
  cout << "Sequential Time: " << t1 << "secs." << endl;
  cout << "Parallel Time: " << t3 << "secs." << endl;

  /* Speed and Efficiency */
  double s = t1/t2;
  double e = s/5;
  cout << "Speed: " << s << endl;
  cout << "Efficiency: " << e << endl;

  return 0;
}