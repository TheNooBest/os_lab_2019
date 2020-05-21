struct SumArgs {
  int *array;
  int begin;
  int end;
  int thread_num;
  int* results;
};

int Sum(const struct SumArgs* args);