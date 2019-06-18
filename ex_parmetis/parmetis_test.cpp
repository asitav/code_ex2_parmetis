# include <cstddef>
# include <ctime>
# include <iostream>
# include <cassert>
# include <fstream>

using namespace std;

# include <metis.h>
//#include <GKlib.h>
#include <parmetis.h>

int main (int argc, char *argv[]);
void partgraphkway_test ( );
void timestamp ( );

//****************************************************************************80

int main (int argc, char *argv[])

//****************************************************************************80
//
//  Purpose:
//
//    ParMETIS_TEST tests the ParMETIS library.
{
  // Init MPI
  MPI_Comm comm;
  idx_t mpi_rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_dup(MPI_COMM_WORLD, &comm);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  if (mpi_rank == 0)
  {
    timestamp();
    cout << "\n";
    cout << "ParMETIS_TEST\n";
    cout << "  C++ version\n";
    cout << "  Test the ParMETIS library for graph partitioning.\n";
  }

  // Main partition call
  partgraphkway_test();

  // Terminate.
  if (mpi_rank == 0)
  {
    cout << "\n";
    cout << "ParMETIS_TEST\n";
    cout << "  Normal end of execution.\n";
    cout << "\n";
    timestamp();
  }

  // Finalize MPI
  MPI_Comm_free(&comm);
  MPI_Finalize();

  return EXIT_SUCCESS;
}
//****************************************************************************80

void partgraphkway_test ( )

/*
 *****************************************************************************80
 *  Purpose:
 *
 *    PARTGRAPHKWAY_TEST tests ParMETIS_V3_PartKway
 *
 *  Discussion:
 *
 *    The graph has the following form:
 *
 *      0 ---  1 ---  2 ---  3 ---  4   <-- Proc 0
 *      |      |      |      |      |
 *      5 ---  6 ---  7 ---  8 ---  9   <-- Proc 1
 *      |      |      |      |      |
 *     10 --- 11 --- 12 --- 13 --- 14   <-- Proc 2
 *
 */
{
  idx_t mpi_rank, mpi_size;
  MPI_Comm comm;
  MPI_Comm_dup(MPI_COMM_WORLD, &comm);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

  assert(mpi_size == 3); // Valid for only 3 procs

  // The number of vertices for each proc.
  idx_t nvtxs = 5; // total 15;

  // Number of balancing constraints, which must be at least 1.
  idx_t ncon = 1;

  // vtxdist array: range of vertices local to each proc, unique for all procs
  idx_t vtxdist[mpi_size + 1];
  vtxdist[0] =  0;
  vtxdist[1] =  5;
  vtxdist[2] = 10;
  vtxdist[3] = 15;

  // Define xadj, and adjncy arrays for each proc
  idx_t *xadj, *adjncy;

  if (mpi_rank == 0)
  {
    // Pointers to initial entries in adjacency array.
    xadj = new idx_t [nvtxs+1];
    xadj[0] = 0;
    xadj[1] = 2;
    xadj[2] = 5;
    xadj[3] = 8;
    xadj[4] = 11;
    xadj[5] = 13;

    // Adjacent vertices in consecutive index order.
    idx_t adjncy_size = 13;
    adjncy = new idx_t [adjncy_size];
    adjncy[ 0] = 1;
    adjncy[ 1] = 5;
    adjncy[ 2] = 0;
    adjncy[ 3] = 2;
    adjncy[ 4] = 6;
    adjncy[ 5] = 1;
    adjncy[ 6] = 3;
    adjncy[ 7] = 7;
    adjncy[ 8] = 2;
    adjncy[ 9] = 4;
    adjncy[10] = 8;
    adjncy[11] = 3;
    adjncy[12] = 9;
  }
  else if (mpi_rank == 1)
  {
    // Pointers to initial entries in adjacency array.
    xadj = new idx_t [nvtxs+1];
    xadj[0] = 0;
    xadj[1] = 3;
    xadj[2] = 7;
    xadj[3] = 11;
    xadj[4] = 15;
    xadj[5] = 18;

    // Adjacent vertices in consecutive index order.
    idx_t adjncy_size = 18;
    adjncy = new idx_t [adjncy_size];
    adjncy[ 0] =  0;
    adjncy[ 1] =  6;
    adjncy[ 2] = 10;
    adjncy[ 3] =  1;
    adjncy[ 4] =  5;
    adjncy[ 5] =  7;
    adjncy[ 6] = 11;
    adjncy[ 7] =  2;
    adjncy[ 8] =  6;
    adjncy[ 9] =  8;
    adjncy[10] = 12;
    adjncy[11] =  3;
    adjncy[12] =  7;
    adjncy[13] =  9;
    adjncy[14] = 13;
    adjncy[15] =  4;
    adjncy[16] =  8;
    adjncy[17] = 14;
  }
  else if (mpi_rank == 2)
  {
    // Pointers to initial entries in adjacency array.
    xadj = new idx_t [nvtxs+1];
    xadj[0] = 0;
    xadj[1] = 2;
    xadj[2] = 5;
    xadj[3] = 8;
    xadj[4] = 11;
    xadj[5] = 13;

    // Adjacent vertices in consecutive index order.
    idx_t adjncy_size = 13;
    adjncy = new idx_t [adjncy_size];
    adjncy[ 0] =  5;
    adjncy[ 1] = 11;
    adjncy[ 2] =  6;
    adjncy[ 3] = 10;
    adjncy[ 4] = 12;
    adjncy[ 5] =  7;
    adjncy[ 6] = 11;
    adjncy[ 7] = 13;
    adjncy[ 8] =  8;
    adjncy[ 9] = 12;
    adjncy[10] = 14;
    adjncy[11] =  9;
    adjncy[12] = 13;
  }

  // The number of parts requested for the partition.
  // nparts in independent of the number of procs used.
  idx_t nparts = 3;

  // On return, the partition vector for the graph.
  // Size: locally-stored vertices
  idx_t part[nvtxs];

  if (mpi_rank == 0)
    cout << "\n"
         << "PARTGRAPHKWAY_TEST:\n"
         << "  ParMETIS_V3_PartKway partitions a graph into K parts\n"
         << "  using multilevel K-way partition.\n";

  //old int ret = METIS_PartGraphKway ( &nvtxs, &ncon, xadj, adjncy, NULL, NULL,
  //old   NULL, &nParts, NULL, NULL, NULL, &objval, part );

  idx_t *vwgt = NULL;
  idx_t *adjwgt = NULL;
  idx_t wgtflag = 3;
  idx_t numflag = 0;
  real_t *tpwgts = NULL;
  real_t *ubvec = NULL;
  idx_t options[1];
  options[0] = 0; // use default values
  idx_t edgecut;

  // Set vwgt, adjwgt
  vwgt = new idx_t [nvtxs*ncon];
  adjwgt = new idx_t [ xadj[nvtxs] ];
  for (int i = 0; i < nvtxs*ncon; i++) vwgt[i] = 1.0;
  for (int i = 0; i < xadj[nvtxs]; i++) adjwgt[i] = 1.0;

  // Set tpwgts
  tpwgts = new real_t [ncon*nparts];
  for (int ipart = 0; ipart < nparts; ipart++)
  {
    for (int icon = 0; icon < ncon; icon++)
    {
      tpwgts[icon + ipart*ncon] = 1.0/(real_t)nparts;
    }
  }

  // Set ubvec
  ubvec = new real_t [ncon];
  for (int icon = 0; icon < ncon; icon++) ubvec[icon] = 1.05;

  ParMETIS_V3_PartKway(vtxdist, xadj, adjncy, vwgt, adjwgt, &wgtflag, &numflag,
      &ncon, &nparts, tpwgts, ubvec, options, &edgecut, part, &comm);

  // Save partition info in files
  std::string path = "part_"+std::to_string(mpi_rank)+".txt";
  std::ios_base::openmode opt = std::ios_base::out;
  std::fstream pfptr;
  pfptr.open(path.c_str(), opt); // partition file streaming ptr
  pfptr << "\n Rank = " << mpi_rank << "\n"
    << "  Edge cuts for partition = " << edgecut << "\n"
    << "  Partition vector:\n"
    << "  Node  Part\n";
  for ( idx_t part_i = 0; part_i < nvtxs; part_i++ )
  {
    pfptr << "     " << part_i << "     " << part[part_i] << std::endl;
  }

  delete [] xadj;
  delete [] adjncy;
  delete [] tpwgts;
  delete [] ubvec;

  return;
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
