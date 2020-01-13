# include <cstddef>
# include <ctime>
# include <iostream>
# include <cassert>
# include <fstream>

using namespace std;

#include <metis.h>
#include <parmetis.h>

int main (int argc, char *argv[]);
void partgraphkwayIO_test ( );
void timestamp ( );

//****************************************************************************80

/*
 *****************************************************************************80
 *
 *  Purpose:
 *
 *    ParMETIS_TEST tests the ParMETIS library.
 */
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
    cout << "\n";
    cout << "ParMETIS_TEST\n";
    cout << "  C++ version\n";
    cout << "  Test the ParMETIS library for graph partitioning.\n";
  }

  partgraphkwayIO_test();

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
} // main

//****************************************************************************80

/*
 *****************************************************************************80
 *  Purpose:
 *
 *    PARTGRAPHKWAYIO_TEST tests ParMETIS_V3_PartKway using graph IO
 *
 *  Discussion:
 *
 *    The graph has the following form, which is read from a graph file.
 *
 *      0 ---  1 ---  2 ---  3 ---  4   <-- Proc 0
 *      |      |      |      |      |
 *      5 ---  6 ---  7 ---  8 ---  9   <-- Proc 1
 *      |      |      |      |      |
 *     10 --- 11 --- 12 --- 13 --- 14   <-- Proc 2
 *
 */
void partgraphkwayIO_test()
{
  idx_t mpi_rank, mpi_size;
  MPI_Comm comm;
  MPI_Comm_dup(MPI_COMM_WORLD, &comm);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

} // partgraphkwayIO_test

/*
 *****************************************************************************80
 *
 *  Purpose:
 *
 *    TIMESTAMP prints the current YMDHMS date as a time stamp.
 *
 *  Example:
 *
 *    31 May 2001 09:45:54 AM
 *
 *  Licensing:
 *
 *    This code is distributed under the GNU LGPL license.
 *
 *  Modified:
 *
 *    08 July 2009
 *
 *  Author:
 *
 *    John Burkardt
 *
 *  Parameters:
 *
 *    None
 *
 */
void timestamp ( )
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
