#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#include "topology.h"
#include "filters.h"

#define N 8

// int adiacent_matrix[N][N] = {
//   {0, 1, 1, 0, 0, 0 ,0, 1}, // 0
//   {1, 0, 0, 1, 1, 1, 0, 0}, // 1
//   {1, 0, 0, 0, 0, 0, 1, 0}, // 2
//   {0, 1, 0, 0, 0, 0, 0, 0}, // 3
//   {0, 1, 0, 0, 0, 1, 0, 0}, // 4
//   {0, 1, 0, 0, 1, 0, 0, 0}, // 5
//   {0, 0, 1, 0, 0, 0, 0, 0}, // 6
//   {1, 0, 0, 0, 0, 0, 0, 0}  // 7
// };

int adiacent_matrix[N][N] = {
  {0, 1, 0, 0, 0, 0 ,0, 0}, // 0
  {1, 0, 0, 1, 0, 0, 0, 0}, // 1
  {0, 0, 0, 1, 0, 0, 0, 0}, // 2
  {0, 1, 1, 0, 1, 0, 0, 0}, // 3
  {0, 0, 0, 1, 0, 1, 1, 1}, // 4
  {0, 0, 0, 0, 1, 0, 0, 0}, // 5
  {0, 0, 0, 0, 1, 0, 0, 0}, // 6
  {0, 0, 0, 0, 1, 0, 0, 0}  // 7
};


int main(int argc, char * argv[])
{
  int rank, nProcesses;
  int parent = -1, i, j;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
  int ** adjacent_matrix = alloc_2d_int(N, N);
  int *  lines_processed = alloc_1d_int(N);
  int ** null_matrix = alloc_2d_int(N, N);


  for (i = 0; i < N; ++i) {
    for (j = 0; j < N; ++j) {
      if (i == rank) adjacent_matrix[i][j] = adiacent_matrix[i][j];
      else adjacent_matrix[i][j] = 0;

      null_matrix[i][j] = 0;
    }
  }

  enum filter_types filter = SHARPEN;
  char * input_image  = "pics/vled.pgm";
  char * output_image = "output.pgm";
  find_topology(adjacent_matrix, N, rank, null_matrix, &parent);
  apply_filter(adjacent_matrix, N, rank, parent, input_image, output_image, filter);

  // int neighbors_size, no_echos = 0;
  // int * neighbors = get_neighbors(adjacent_matrix, N, rank, &neighbors_size);
  //
  // if (rank != 0) {
  //   int tag;
  //   parent = receive_sonda_ecou(adjacent_matrix, N, MPI_ANY_SOURCE, &tag);
  // }
  //
  // // Send sondaj
  // for (i = 0; i < neighbors_size; ++i) {
  //   if (neighbors[i] != parent) {
  //     send_sonda_ecou(SONDAJ, neighbors[i], null_matrix, N);
  //     no_echos ++;
  //   }
  // }
  //
  // // Wait for echos
  // while (no_echos > 0) {
  //   int tag;
  //   int source = receive_sonda_ecou(adjacent_matrix, N, MPI_ANY_SOURCE, &tag);
  //   if (tag == EMPTY_ECHO) {
  //     adjacent_matrix[rank][source] = 0;
  //   }
  //   if (tag == SONDAJ) {
  //     send_sonda_ecou(EMPTY_ECHO, source, null_matrix, N);
  //   } else {
  //     no_echos--;
  //   }
  // }
  //
  // // Propaga in sus
  // if (rank != 0) {
  //   send_sonda_ecou(ECHO, parent, adjacent_matrix, N);
  // }




  dealloc_2d_int(adjacent_matrix);
  dealloc_2d_int(null_matrix);

  // parent must have the solution


  // Send images

  // if (p <> inițiator)
  //     receive sondă-ecou[p](k, top_nou);
  //     mark as  parent source
  //
  //  /* transmite sondaje celorlalte noduri vecine, copiii lui p */
  //  for [q = 1 to N st (leg[q] AND (q <> părinte))] {
  //     send sondă-ecou[q](sondă, p, O);  /* topologie nulă */
  //     nr_ecouri = nr_ecouri + 1;
  //  }
  // while (nr_ecouri > 0) {
  //     receive sondă-ecou[p](k, transm, top_nou);
  //
  ////// fara sonda
  //     if (k == sondă)
  //        send sondă-ecou[transm](ecou, p, O);
  //     else if (k == ecou) {
  //        top = top OR top_nou;
  //        nr_ecouri = nr_ecouri – 1;
  //     }
  //  }
  //
  //  if (p <> inițiator)
  //     send sondă-ecou[părinte](ecou, p, top);

  // if (rank == 0) {
  //   for (i = 0; i < N; ++i) {
  //     printf("Leaf:%d, %d : ",is_leaf(adjacent_matrix, N, i), i);
  //     for (j = 0; j < N; ++j) {
  //       if (adjacent_matrix[i][j] != adiacent_matrix[i][j]) {
  //         // printf("error\n" );
  //       }
  //       if (adjacent_matrix[i][j]) {
  //         printf("%d ", j);
  //       } else {
  //         // printf("%d ", adjacent_matrix[i][j]);
  //       }
  //     }
  //     printf("\n" );
  //   }
  // }

  MPI_Finalize();
  return 0;
}
