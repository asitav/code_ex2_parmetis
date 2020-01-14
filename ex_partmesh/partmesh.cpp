# include <cstddef>
# include <ctime>
# include <iostream>
# include <cassert>
# include <fstream>

#define MAXNCON 32
#define IMBALANCE_TOLERANCE 1.05
#define PMV3_OPTION_DBGLVL 1
#define PMV3_OPTION_SEED 2

using namespace std;

#include <metis.h>
#include <parmetis.h>

int main (int argc, char *argv[]);
void partMeshKway_proc1_tri2();
void partMeshKway_proc1_trihex6();
void partMeshKway_proc3_trihex6();
void timestamp ( );

//****************************************************************************80


//****************************************************************************80
//
//  Purpose:
//
//    ParMETIS_TEST tests the ParMETIS library.
int main (int argc, char *argv[])
{
  // Init MPI
  MPI_Comm comm;
  idx_t mpi_rank, mpi_size;
  MPI_Init(&argc, &argv);
  MPI_Comm_dup(MPI_COMM_WORLD, &comm);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

  if (mpi_rank == 0)
  {
    timestamp();
    cout << "\n ParMETIS test for direct mesh partitioning.\n";
  }

  // Main partition call
  if (mpi_size > 1)
  {
    partMeshKway_proc3_trihex6();
  }
  else if (mpi_size == 1)
  {
    //partMeshKway_proc1_tri2();
    partMeshKway_proc1_trihex6();
  }

  // Terminate.
  if (mpi_rank == 0)
  {
    cout << "\nEnd ParMETIS_TEST\n";
    timestamp();
  }

  // Finalize MPI
  MPI_Comm_free(&comm);
  MPI_Finalize();

  return EXIT_SUCCESS;
} // main

//****************************************************************************80
/*
 *
 * Two tri mesh:
 *
 *     1
 *    /|\
 *   / | \
 *  /  |  \
 * 0 e0|e1 3
 *  \  |  /
 *   \ | /
 *    \|/
 *     2
 */
void partMeshKway_proc1_tri2()
{
  idx_t mpi_rank, mpi_size;
  MPI_Comm comm;
  MPI_Comm_dup(MPI_COMM_WORLD, &comm);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

  idx_t *elmwgt = nullptr;
  idx_t wgtflag = 0;
  idx_t numflag = 0;
  idx_t ncon = 1;
  idx_t ncommnodes = 2;
  idx_t nparts = 2; // fix to 2 parts now ...
  idx_t nelms = 2; //  each proc (proc 0 only here ) has 2 elements
  //idx_t esizemax = 3; // max num of nodes per elm (3 for tri)
  idx_t options[10];
  idx_t edgecut;
  idx_t part [nelms];

  real_t *tpwgts, ubvec[MAXNCON];
  tpwgts = new real_t [ nparts * ncon ];
  for (int i = 0; i < nparts * ncon; ++i) tpwgts[i] = 1.0/(real_t) (nparts);
  for (int i = 0; i < ncon; ++i) ubvec[i] = IMBALANCE_TOLERANCE;

  options[0] = 1;
  options[PMV3_OPTION_DBGLVL] = 7;
  options[PMV3_OPTION_SEED] = 0;

  // elmdist array: range of elements local to each proc, same for all procs
  idx_t elmdist[] = {0, 2}; // 2 = ntotal elms{0, 1} //size = mpi_size + 1

  // Pointers to initial entries in eind (element node list) array
  idx_t eptr[] = {0, 3, 6}; // size = nelms + 1

  // Adjacency array: node list stored in elm by elm manner
  // eind[eptr[i]: eptr[i+1]-1] saves nodes of elm i
  idx_t eind [] = {0, 2, 1,   1, 2, 3}; // size  nelms*esizemax

  ParMETIS_V3_PartMeshKway(elmdist, eptr, eind, elmwgt, &wgtflag,
      &numflag, &ncon, &ncommnodes, &nparts, tpwgts, ubvec, options,
      &edgecut, part, &comm);

  // Save partition info in files
  std::string path = "part_proc1_"+std::to_string(mpi_rank)+"_tri.txt";
  std::cout << "Writing partition info into file: " << path << "\n";
  std::ios_base::openmode opt = std::ios_base::out;
  std::fstream pfptr;
  pfptr.open(path.c_str(), opt); // partition file streaming ptr
  pfptr << "\n Rank = " << mpi_rank << "\n"
    << "  Edge cuts for partition = " << edgecut << "\n"
    << "  Partition vector:\n"
    << "  Element  Part\n";
  for ( idx_t part_i = 0; part_i < nelms; part_i++ )
  {
    pfptr << "     " << part_i << "     " << part[part_i] << std::endl;
  }

  delete [] tpwgts;
}

//****************************************************************************80
/*
 *
 *  Tri/Qua hybrid mesh:
 *
 *     0---1
 *    /|   |\
 *   / |   | \
 *  /e0|e1 |e2\
 * 2---3---4---5
 * |   |   |   |
 * | e3|e4 |e5 |
 * |   |   |   |
 * 6---7---8---9
 */
void partMeshKway_proc1_trihex6()
{
  idx_t mpi_rank, mpi_size;
  MPI_Comm comm;
  MPI_Comm_dup(MPI_COMM_WORLD, &comm);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  if (mpi_size > 1) { cout << " Nprocs > 1; Exiting!! \n"; throw 10;}

  idx_t *elmwgt = nullptr;
  idx_t wgtflag = 0;
  idx_t numflag = 0;
  idx_t ncon = 1;
  idx_t ncommnodes = 2;
  idx_t nparts = 2; // fix to 2 parts now ...
  idx_t nelms = 6; //  each proc (proc 0 only here ) has 6 elements
  //idx_t esizemax = 3; // max num of nodes per elm (3 for tri)
  idx_t options[10];
  idx_t edgecut;
  idx_t part [nelms];

  real_t *tpwgts, ubvec[MAXNCON];
  tpwgts = new real_t [ nparts * ncon ];
  for (int i = 0; i < nparts * ncon; ++i) tpwgts[i] = 1.0/(real_t) (nparts);
  for (int i = 0; i < ncon; ++i) ubvec[i] = IMBALANCE_TOLERANCE;

  options[0] = 1;
  options[PMV3_OPTION_DBGLVL] = 7;
  options[PMV3_OPTION_SEED] = 0;

  // elmdist array: range of elements local to each proc, same for all procs
  idx_t elmdist[] = {0, 6}; // 6 = ntotal elms{0, 1, ..., 5} //size = mpi_size + 1

  // Pointers to initial entries in eind (element node list) array
  idx_t eptr[] = {0, 3, 7, 10, 14, 18, 22}; // size = nelms + 1

  // Adjacency array: node list stored in elm by elm manner
  // eind[eptr[i]: eptr[i+1]-1] saves nodes of elm i
  idx_t eind [] = {0, 2, 3,     // elm 0
                   1, 0, 3, 4,  // elm 1
                   1, 4, 5,     // elm 2
                   3, 2, 6, 7,  // elm 3
                   4, 3, 7, 8,  // elm 4
                   5, 4, 8, 9   // elm 5
                  };

  ParMETIS_V3_PartMeshKway(elmdist, eptr, eind, elmwgt, &wgtflag,
      &numflag, &ncon, &ncommnodes, &nparts, tpwgts, ubvec, options,
      &edgecut, part, &comm);

  // Save partition info in files
  std::string path = "part_proc1_"+std::to_string(mpi_rank)+"_triQua.txt";
  std::cout << "Writing partition info into file: " << path << "\n";
  std::ios_base::openmode opt = std::ios_base::out;
  std::fstream pfptr;
  pfptr.open(path.c_str(), opt); // partition file streaming ptr
  pfptr << "\n Rank = " << mpi_rank << "\n"
    << "  Edge cuts for partition = " << edgecut << "\n"
    << "  Partition vector:\n"
    << "  Element  Part\n";
  for ( idx_t part_i = 0; part_i < nelms; part_i++ )
  {
    pfptr << "     " << part_i << "     " << part[part_i] << std::endl;
  }

  delete [] tpwgts;
}

//****************************************************************************80
/*
 *
 *  Tri/Qua hybrid mesh:
 *
 *     0---1
 *    /|   |\
 *   / |   | \
 *  /e0|e1 |e2\
 * 2---3---4---5
 * |   |   |   |
 * | e3|e4 |e5 |
 * |   |   |   |
 * 6---7---8---9
 */
void partMeshKway_proc3_trihex6()
{
  idx_t mpi_rank, mpi_size;
  MPI_Comm comm;
  MPI_Comm_dup(MPI_COMM_WORLD, &comm);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  if (mpi_size != 3) { cout << " Nprocs is not 3; Exiting!! \n"; throw 11;}

  idx_t *elmwgt = nullptr;
  idx_t nelms, nnodes;
  idx_t wgtflag = 0;
  idx_t numflag = 0;
  idx_t ncon = 1;
  idx_t ncommnodes = 2;
  idx_t nparts = 2; // fix to 2 parts now ...
  //idx_t esizemax = 3; // max num of nodes per elm (3 for tri)
  idx_t options[10];
  idx_t edgecut;

  real_t *tpwgts = nullptr, ubvec[MAXNCON];
  tpwgts = new real_t [ nparts * ncon ];
  for (int i = 0; i < nparts * ncon; ++i) tpwgts[i] = 1.0/(real_t) (nparts);
  for (int i = 0; i < ncon; ++i) ubvec[i] = IMBALANCE_TOLERANCE;

  options[0] = 1;
  options[PMV3_OPTION_DBGLVL] = 7;
  options[PMV3_OPTION_SEED] = 0;

  // elmdist array: range of elements local to each proc, same for all procs
  idx_t elmdist[] = {0, 2, 4, 6}; // 6 = ntotal elms{0, 1, ..., 5} //size = mpi_size + 1

  idx_t *eptr = nullptr, *eind = nullptr;
  if (mpi_rank == 0)
  {
    nelms = 2;
    nnodes = 7;

    // Pointers to initial entries in eind (element node list) array
    eptr = new idx_t [nelms + 1];
    eptr[0] = 0;
    eptr[1] = 3;
    eptr[2] = 7; // nnodes

    // Adjacency array: node list stored in elm by elm manner
    // eind[eptr[i]: eptr[i+1]-1] saves nodes of elm i
    eind = new idx_t [nnodes];
    eind[0] = 0; // elm 0
    eind[1] = 2; // ...
    eind[2] = 3; // ...
    eind[3] = 1; // elm 1
    eind[4] = 0; // ...
    eind[5] = 3; // ...
    eind[6] = 4; // ...
  }
  else if (mpi_rank == 1)
  {
    nelms = 2;
    nnodes = 7;

    // Pointers to initial entries in eind (element node list) array
    eptr = new idx_t [nelms + 1];
    eptr[0] = 0;
    eptr[1] = 3;
    eptr[2] = 7; // nnodes

    // Adjacency array: node list stored in elm by elm manner
    // eind[eptr[i]: eptr[i+1]-1] saves nodes of elm i
    eind = new idx_t [nnodes];
    eind[0] = 1; // elm 0
    eind[1] = 4; // ...
    eind[2] = 5; // ...
    eind[3] = 3; // elm 1
    eind[4] = 2; // ...
    eind[5] = 6; // ...
    eind[6] = 7; // ...
  }
  else if (mpi_rank == 2)
  {
    nelms = 2;
    nnodes = 8;


    // Pointers to initial entries in eind (element node list) array
    eptr = new idx_t [nelms + 1];
    eptr[0] = 0;
    eptr[1] = 4;
    eptr[2] = 8; // nnodes

    // Adjacency array: node list stored in elm by elm manner
    // eind[eptr[i]: eptr[i+1]-1] saves nodes of elm i
    eind = new idx_t [nnodes];
    eind[0] = 4; // elm 0
    eind[1] = 3; // ...
    eind[2] = 7; // ...
    eind[3] = 8; // ...
    eind[4] = 5; // elm 1
    eind[5] = 4; // ...
    eind[6] = 8; // ...
    eind[7] = 9; // ...
  }
  else
  {
    cout << "mpi_rank > 2 not possible! Exiting!!!\n";
    throw 12;
  }

  idx_t part [nelms];

  ParMETIS_V3_PartMeshKway(elmdist, eptr, eind, elmwgt, &wgtflag,
      &numflag, &ncon, &ncommnodes, &nparts, tpwgts, ubvec, options,
      &edgecut, part, &comm);

  // Save partition info in files
  std::string path = "part_proc3_"+std::to_string(mpi_rank)+"_triQua.txt";
  std::cout << "Writing partition info into file: " << path << "\n";
  std::ios_base::openmode opt = std::ios_base::out;
  std::fstream pfptr;
  pfptr.open(path.c_str(), opt); // partition file streaming ptr
  pfptr << "\n Rank = " << mpi_rank << "\n"
    << "  Edge cuts for partition = " << edgecut << "\n"
    << "  Partition vector:\n"
    << "  Element  Part\n";
  for ( idx_t part_i = 0; part_i < nelms; part_i++ )
  {
    pfptr << "     " << part_i << "     " << part[part_i] << std::endl;
  }

  delete [] tpwgts;
  delete [] eptr  ;
  delete [] eind  ;
}

//****************************************************************************80

void timestamp ( )

//****************************************************************************80
//
//  Purpose:
//
//    TIMESTAMP prints the current YMDHMS date as a time stamp.
//
//  Example:
//
//    31 May 2001 09:45:54 AM
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    08 July 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    None
//
{
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct std::tm *tm_ptr;
  size_t len;
  std::time_t now;

  now = std::time ( NULL );
  tm_ptr = std::localtime ( &now );

  len = std::strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm_ptr );

  if (len > 0) std::cout << time_buffer << "\n";

  return;
# undef TIME_SIZE
}
