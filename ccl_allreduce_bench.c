/*
 Copyright 2016-2019 Intel Corporation
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
     http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

#include "base.h"
#include "stdlib.h"
#include "string.h"
#define RUN_COLLECTIVE(start_cmd, count, name)				\
  do {                                                                     \
      t = 0;                                                               \
      for (iter_idx = 0; iter_idx < ITERS; iter_idx++)                     \
      {                                                                    \
          for (idx = 0; idx < count; idx++)                                \
          {                                                                \
              send_buf[idx] = (float)rank;                                 \
              recv_buf[idx] = 0.0;                                         \
          }                                                                \
          t1 = when();                                                     \
          CCL_CALL(start_cmd);                                             \
          CCL_CALL(ccl_wait(request));                                     \
          t2 = when();                                                     \
          t += (t2 - t1);                                                  \
      }                                                                    \
      ccl_barrier(NULL, NULL);                                             \
      float expected = (size - 1) * ((float)size / 2);                     \
      for (idx = 0; idx < count; idx++)                                    \
      {                                                                    \
          if (recv_buf[idx] != expected)                                   \
          {                                                                \
              printf("iter %zu, idx %zu, expected %f, got %f\n",           \
                      iter_idx, idx, expected, recv_buf[idx]);             \
              ASSERT(0, "unexpected value");                               \
          }                                                                \
      }                                                                    \
      printf("[%zu] %ld bytes - avg %s time: %8.2lf us\n", rank, count*sizeof(float), name, t / ITERS); \
      fflush(stdout);                                                      \
  } while (0)

int main(int argc, char *argv[])
{
  int i=0;
  uint count = 1024;
  while(i<argc) {
    if (strcmp(argv[i], "--count")==0) {
      count = atoi(argv[i+1]);
      i=i+2;
    } else {
      i=i+1;
    }
  }
  float* send_buf = (float*) malloc(sizeof(float) * count);
  float* recv_buf = (float*) malloc(sizeof(float) * count);
  
    test_init();

    coll_attr.to_cache = 0;
    RUN_COLLECTIVE(ccl_allreduce(send_buf, recv_buf, count, ccl_dtype_float, ccl_reduction_sum, &coll_attr, NULL, NULL, &request), count, 
                   "warmup_allreduce");

    coll_attr.to_cache = 1;
    RUN_COLLECTIVE(ccl_allreduce(send_buf, recv_buf, count, ccl_dtype_float, ccl_reduction_sum, &coll_attr, NULL, NULL, &request), count, 
                   "persistent_allreduce");

    coll_attr.to_cache = 0;
    RUN_COLLECTIVE(ccl_allreduce(send_buf, recv_buf, count, ccl_dtype_float, ccl_reduction_sum, &coll_attr, NULL, NULL, &request),count,
                   "regular_allreduce");

    test_finalize();

    free(send_buf);
    free(recv_buf);

    if (rank == 0)
        printf("PASSED\n");

    return 0;
}
