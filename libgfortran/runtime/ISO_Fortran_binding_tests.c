/* Test suite for ISO_Fortran_binding.h */
#include "ISO_Fortran_binding.h"
#include <stdio.h>
#include <stdlib.h>

int main (void)
{

  CFI_rank_t      rank;
  CFI_attribute_t attribute;
  CFI_type_t      type[10] = {CFI_type_Bool,        CFI_type_short,
                         CFI_type_ucs4_char,   CFI_type_double,
                         CFI_type_float128,    CFI_type_float128_Complex,
                         CFI_type_long_double, CFI_type_long_double_Complex,
                         CFI_type_struct,      CFI_type_other};
  size_t elem_len;
  int    ind;
  size_t base_type;
  size_t base_type_size;
  size_t errno;

  /* Test function establish. */
  /* Fresh descriptor, base address is NULL. */
  printf ("Test CFI_establish: dv.base_addr == NULL.\n\n");
  /* Loop through type. */
  for (int i = 0; i < 10; i++)
    {
      elem_len = 0;
      if (type[i] != CFI_type_struct && type[i] != CFI_type_other)
        {
          base_type      = type[i] & CFI_type_mask;
          base_type_size = (type[i] - base_type) >> CFI_type_kind_shift;
        }
      else
        {
          base_type      = type[i];
          base_type_size = elem_len;
        }
      /* Loop through attribute. */
      for (int j = 1; j <= 3; j++)
        {
          attribute = j;
          /* Loop through rank. */
          for (int k = 0; k <= CFI_MAX_RANK; k++)
            {
              errno = 1;
              rank  = k;
              CFI_CDESC_T (rank) test1;
              /* We do this because C sometimes doesn't make the structures with
               * a null base_addr which leads to weird behaviour inside
               * CFI_establish.
               */
              if (test1.base_addr != NULL)
                {
                  test1.base_addr = NULL;
                  free (test1.base_addr);
                }
              ind = CFI_establish ((CFI_cdesc_t *)&test1, NULL, attribute,
                                   type[i], elem_len, rank, NULL);
              printf ("attribute = %d\ntype = %d\nbase_type = %ld\nrank = "
                      "%d\nelem_len = %ld\n",
                      attribute, type[i], base_type, rank, elem_len);
              if (ind != CFI_SUCCESS)
                {
                  printf ("CFI_establish return value = %d\n", ind);
                  errno *= 2;
                  printf ("errno = %ld\n\n", errno);
                  goto next_attribute1;
                }
              if (attribute != test1.attribute)
                {
                  printf ("Attribute fail.\n");
                  errno *= 3;
                }
              if (type[i] != test1.type)
                {
                  printf ("Type fail.\n");
                  errno *= 5;
                }
              if (rank != test1.rank)
                {
                  printf ("Rank fail.\n");
                  errno *= 7;
                }

              elem_len = base_type_size;
              if (base_type_size == 10)
                {
                  elem_len = 64;
                }
              if (base_type == CFI_type_Complex)
                {
                  elem_len *= 2;
                }
              if (elem_len != test1.elem_len)
                {
                  printf ("Element length fail: type_idx = %d., elem_len = "
                          "%ld\n",
                          i, elem_len);
                  errno *= 11;
                }
              printf ("errno = %ld\n\n", errno);
            }
        next_attribute1:;
        }
    }

  /* Fresh descriptor, base address is not NULL */
  printf ("Test CFI_establish: dv.base_addr != NULL.\n\n");
  CFI_index_t *extents = NULL;
  /* Loop through type. */
  for (int i = 0; i < 10; i++)
    {
      elem_len = 0;
      if (type[i] != CFI_type_struct && type[i] != CFI_type_other)
        {
          base_type      = type[i] & CFI_type_mask;
          base_type_size = (type[i] - base_type) >> CFI_type_kind_shift;
        }
      else
        {
          base_type      = type[i];
          base_type_size = elem_len;
        }
      /* Loop through attribute. */
      for (int j = 1; j <= 3; j++)
        {
          attribute = j;
          /* Loop through rank. */
          for (int k = 0; k <= CFI_MAX_RANK; k++)
            {
              errno = 1;
              rank  = k;
              if (extents != NULL)
                {
                  free (extents);
                }
              extents = malloc (rank * sizeof (CFI_index_t));
              for (int r = 0; r < rank; r++)
                {
                  extents[r] = r + 1;
                }
              CFI_CDESC_T (rank) test2;
              /* We do this because C sometimes doesn't make the structures with
               * a null base_addr which leads to weird behaviour inside
               * CFI_establish.
               */
              if (test2.base_addr != NULL)
                {
                  test2.base_addr = NULL;
                  free (test2.base_addr);
                }
              ind = CFI_establish ((CFI_cdesc_t *)&test2, &ind, attribute,
                                   type[i], elem_len, rank, extents);
              printf ("attribute = %d\ntype = %d\nbase_type = %ld\nrank = "
                      "%d\nelem_len = %ld\n",
                      attribute, type[i], base_type, rank, elem_len);
              if (ind != CFI_SUCCESS)
                {
                  printf ("CFI_establish return value = %d\n", ind);
                  errno *= 2;
                  printf ("errno = %ld\n\n", errno);
                  goto next_attribute2;
                }
              if (attribute != test2.attribute)
                {
                  printf ("Attribute fail.\n");
                  errno *= 3;
                }
              if (type[i] != test2.type)
                {
                  printf ("Type fail.\n");
                  errno *= 5;
                }
              if (rank != test2.rank)
                {
                  printf ("Rank fail.\n");
                  errno *= 7;
                }

              elem_len = base_type_size;
              if (base_type_size == 10)
                {
                  elem_len = 64;
                }
              if (base_type == CFI_type_Complex)
                {
                  elem_len *= 2;
                }
              if (elem_len != test2.elem_len)
                {
                  printf ("Element length fail: type_idx = %d., elem_len = "
                          "%ld must be equal.\n",
                          i, elem_len);
                  errno *= 11;
                }

              printf ("extents = [ ");
              for (int r = 0; r < rank; r++)
                {
                  if (extents[r] != test2.dim[r].extent)
                    {
                      printf ("Extents fail: extents[%d] = %ld., "
                              "dv.dim[%d].extent = %ld\n",
                              r, extents[r], r, test2.dim[r].extent);
                      errno *= 13;
                    }
                  printf ("%ld ", extents[r]);
                }
              printf ("]\n");

              if (attribute == CFI_attribute_pointer)
                {
                  printf ("test2.dim[].lower_bound = [ ");
                  for (int r = 0; r < rank; r++)
                    {
                      if (test2.dim[r].lower_bound != 0)
                        {
                          printf (
                              "Dimension lower bound fail: if the attribute is "
                              "for a pointer, the lower bounds of every "
                              "dimension must be zero, "
                              "test2.dim[%d].lower_bound = %ld.\n",
                              r, test2.dim[r].lower_bound);
                          errno *= 17;
                        }
                      printf ("%ld ", test2.dim[r].lower_bound);
                    }
                  printf ("]\n");
                }
              printf ("errno = %ld\n\n", errno);
            }
        next_attribute2:;
        }
    }

  /* Fresh descriptor, base address is not NULL */
  printf ("Test CFI_allocate: dv.base_addr != NULL.\n\n");
  CFI_index_t *lower = NULL;
  CFI_index_t *upper = NULL;
  /* Loop through type. */
  for (int i = 0; i < 10; i++)
    {
      elem_len = 0;
      if (type[i] == CFI_type_struct)
        {
          base_type      = type[i];
          base_type_size = 69;
        }
      else if (type[i] == CFI_type_other)
        {
          base_type      = type[i];
          base_type_size = 666;
        }
      else if (type[i] == CFI_type_char || type[i] == CFI_type_ucs4_char ||
               type[i] == CFI_type_signed_char)
        {
          base_type      = type[i] & CFI_type_mask;
          base_type_size = 3;
        }
      else
        {
          base_type      = type[i] & CFI_type_mask;
          base_type_size = (type[i] - base_type) >> CFI_type_kind_shift;
        }

      elem_len = base_type_size;
      if (base_type_size == 10)
        {
          elem_len = 64;
        }
      if (base_type == CFI_type_Complex)
        {
          elem_len *= 2;
        }
      /* Loop through attribute. */
      for (int j = 1; j <= 3; j++)
        {
          attribute = j;
          /* Loop through rank. */
          for (int k = 0; k <= CFI_MAX_RANK; k++)
            {
              errno = 1;
              rank  = k;
              if (extents != NULL)
                {
                  free (extents);
                }
              if (lower != NULL)
                {
                  free (lower);
                }
              if (upper != NULL)
                {
                  free (upper);
                }
              extents = malloc (rank * sizeof (CFI_index_t));
              lower   = malloc (rank * sizeof (CFI_index_t));
              upper   = malloc (rank * sizeof (CFI_index_t));
              for (int r = 0; r < rank; r++)
                {
                  extents[r] = 2;
                  lower[r]   = r;
                  upper[r]   = lower[r] + extents[r];
                }
              CFI_CDESC_T (rank) test3;
              /* We do this because C sometimes doesn't make the structures with
               * a null base_addr which leads to weird behaviour inside
               * CFI_establish.
               */
              if (test3.base_addr != NULL)
                {
                  test3.base_addr = NULL;
                  free (test3.base_addr);
                }
              ind = CFI_establish ((CFI_cdesc_t *)&test3, NULL, attribute,
                                   type[i], elem_len, rank, extents);
              ind = CFI_allocate ((CFI_cdesc_t *)&test3, lower, upper,
                                  base_type_size);
              printf ("type = %ld\nelem_len = %ld\n", base_type,
                      test3.elem_len);
              if (ind != CFI_SUCCESS)
                {
                  errno *= 2;
                  goto next_attribute3;
                }
              for (int r = 0; r < rank; r++)
                {
                  if (lower[r] != test3.dim[r].lower_bound)
                    {
                      printf (
                          "Dimension lower bound fail: lower[%d] = %ld, "
                          "test3.dim[%d].lower_bound = %ld must be equal.\n",
                          r, lower[r], r, test3.dim[r].lower_bound);
                      errno *= 3;
                    }
                  if (upper[r] - test3.dim[r].lower_bound + 1 !=
                      test3.dim[r].extent)
                    {
                      printf ("Extent fail: upper[%d] - "
                              "test3.dim[%d].lower_bound + 1 = %ld, "
                              "test3.dim[%d].extent = %ld must be equal.\n",
                              r, r, upper[r] - test3.dim[r].lower_bound + 1, r,
                              test3.dim[r].lower_bound);
                      errno *= 5;
                    }
                  if (test3.dim[r].sm != test3.elem_len)
                    {
                      printf ("Memory stride fail: test3.dim[%d].sm = %ld, "
                              "test3.elem_len = %ld must be equal.\n",
                              r, test3.dim[r].sm, test3.elem_len);
                      errno *= 7;
                    }
                }
              if (elem_len != test3.elem_len)
                {
                  printf ("Element length fail: type_idx = %d., elem_len = "
                          "%ld must be equal.\n",
                          i, elem_len);
                  errno *= 11;
                }
            }
        next_attribute3:;
          printf ("errno = %ld\n\n", errno);
        }
    }

  printf ("Test CFI_allocate: dv.base_addr == NULL.\n\n");
  rank  = 1;
  errno = 1;
  CFI_CDESC_T (rank) test4;
  base_type      = type[3] & CFI_type_mask;
  base_type_size = (type[3] - base_type) >> CFI_type_kind_shift;
  attribute      = CFI_attribute_allocatable;
  ind = CFI_establish ((CFI_cdesc_t *)&test4, NULL, attribute, type[3],
                       elem_len, rank, NULL);
  ind = CFI_allocate ((CFI_cdesc_t *)&test4, NULL, NULL, base_type_size);
  if (ind != CFI_INVALID_EXTENT)
    {
      errno *= 2;
    }
  printf ("errno = %ld\n\n", errno);

  printf ("Test CFI_allocate: lower and upper == NULL.\n\n");
  rank  = 1;
  errno = 1;
  CFI_CDESC_T (rank) test5;
  base_type      = type[3] & CFI_type_mask;
  base_type_size = (type[3] - base_type) >> CFI_type_kind_shift;
  attribute      = CFI_attribute_pointer;
  ind = CFI_establish ((CFI_cdesc_t *)&test5, &ind, attribute, type[3],
                       elem_len, rank, extents);
  ind = CFI_allocate ((CFI_cdesc_t *)&test5, NULL, NULL, base_type_size);
  if (ind != CFI_ERROR_BASE_ADDR_NOT_NULL)
    {
      errno *= 2;
    }
  printf ("errno = %ld\n\n", errno);

  /* Test CFI_deallocate */
  printf ("Test CFI_deallocate.\n\n");
  rank           = 1;
  errno          = 1;
  base_type      = type[3] & CFI_type_mask;
  base_type_size = (type[3] - base_type) >> CFI_type_kind_shift;
  for (int i = 1; i <= 3; i++)
    {
      attribute = i;
      if (extents != NULL)
        {
          free (extents);
        }
      if (lower != NULL)
        {
          free (lower);
        }
      if (upper != NULL)
        {
          free (upper);
        }
      extents = malloc (rank * sizeof (CFI_index_t));
      lower   = malloc (rank * sizeof (CFI_index_t));
      upper   = malloc (rank * sizeof (CFI_index_t));
      CFI_CDESC_T (rank) test6;
      ind = CFI_establish ((CFI_cdesc_t *)&test6, NULL, attribute, type[i],
                           elem_len, rank, extents);
      ind = CFI_allocate ((CFI_cdesc_t *)&test6, lower, upper, base_type_size);
      ind = CFI_deallocate ((CFI_cdesc_t *)&test6);
      if (ind != CFI_INVALID_ATTRIBUTE && test6.base_addr != NULL)
        {
          errno *= 2;
          printf ("Deallocation failed.\n");
        }
      printf ("attribute = %d\nerrno = %ld\n\n", test6.attribute, errno);
    }

  printf ("Test CFI_is_contiguous.\n\n");
  int tmp_ind;
  base_type      = type[3] & CFI_type_mask;
  base_type_size = (type[3] - base_type) >> CFI_type_kind_shift;
  for (int i = 1; i <= 3; i++)
    {
      attribute = i;
      for (int j = 0; j <= 4; j++)
        {
          errno = 1;
          rank  = j;
          if (extents != NULL)
            {
              free (extents);
            }
          if (lower != NULL)
            {
              free (lower);
            }
          if (upper != NULL)
            {
              free (upper);
            }
          extents = malloc (rank * sizeof (CFI_index_t));
          lower   = malloc (rank * sizeof (CFI_index_t));
          upper   = malloc (rank * sizeof (CFI_index_t));
          for (int r = 0; r < rank; r++)
            {
              extents[r] = 2;
              lower[r]   = r;
              upper[r]   = lower[r] + extents[r];
            }
          CFI_CDESC_T (rank) test7;
          ind = CFI_establish ((CFI_cdesc_t *)&test7, NULL, attribute, type[3],
                               elem_len, rank, extents);
          tmp_ind = CFI_allocate ((CFI_cdesc_t *)&test7, lower, upper,
                                  base_type_size);
          ind = CFI_is_contiguous ((CFI_cdesc_t *)&test7);
          printf ("attribute = %d\nrank = %d\n", attribute, rank);
          // printf("%d\n",ind);
          if (ind != CFI_INVALID_RANK && rank == 0 &&
              tmp_ind != CFI_INVALID_ATTRIBUTE)
            {
              printf ("CFI_is_contiguous rank failure %d.\n", tmp_ind);
              errno *= 2;
            }
          else if (ind == CFI_ERROR_BASE_ADDR_NULL && test7.base_addr != NULL &&
                   tmp_ind != CFI_SUCCESS)
            {
              printf ("CFI_is_contiguous base addres failure.\n");
              errno *= 3;
            }
          printf ("errno = %ld\n\n", errno);
        }
    }

  printf ("Test CFI_address.\n\n");
  CFI_index_t *tr_subscripts;
  CFI_dim_t *  tr_dim;
  /* Loop through type. */
  for (int i = 0; i < 10; i++)
    {
      elem_len = 0;
      if (type[i] == CFI_type_struct)
        {
          base_type      = type[i];
          base_type_size = 1;
        }
      else if (type[i] == CFI_type_other)
        {
          base_type      = type[i];
          base_type_size = 1;
        }
      else if (type[i] == CFI_type_char || type[i] == CFI_type_ucs4_char ||
               type[i] == CFI_type_signed_char)
        {
          base_type      = type[i] & CFI_type_mask;
          base_type_size = 3;
        }
      else
        {
          base_type      = type[i] & CFI_type_mask;
          base_type_size = (type[i] - base_type) >> CFI_type_kind_shift;
        }

      elem_len = base_type_size;
      if (base_type_size == 10)
        {
          elem_len = 64;
        }
      if (base_type == CFI_type_Complex)
        {
          elem_len *= 2;
        }
      /* Loop through attribute. */
      for (int j = 1; j <= 3; j++)
        {
          attribute = j;
          /* Loop through rank. */
          for (int k = 1; k <= CFI_MAX_RANK; k++)
            {
              errno = 1;
              rank  = k;
              CFI_CDESC_T (rank) source;
              if (extents != NULL)
                {
                  free (extents);
                }
              if (lower != NULL)
                {
                  free (lower);
                }
              if (upper != NULL)
                {
                  free (upper);
                }
              extents = malloc (rank * sizeof (CFI_index_t));
              lower   = malloc (rank * sizeof (CFI_index_t));
              upper   = malloc (rank * sizeof (CFI_index_t));
              for (int r = 0; r < rank; r++)
                {
                  extents[r] = rank - r + 1;
                  lower[r]   = rank - r - 3;
                  // printf("lower[%d] = %ld\n", r, lower[r]);
                  upper[r] = lower[r] + extents[r] - 1;
                }
              ind = CFI_establish ((CFI_cdesc_t *)&source, NULL,
                                   CFI_attribute_allocatable, type[i], elem_len,
                                   rank, extents);
              ind =
                  CFI_allocate ((CFI_cdesc_t *)&source, lower, upper, elem_len);
              if (ind == CFI_SUCCESS)
                {
                  CFI_index_t dif_addr;
                  CFI_index_t n_entries = 1;
                  dif_addr              = (CFI_index_t) (
                      (char *)CFI_address ((CFI_cdesc_t *)&source, upper) -
                      (char *)CFI_address ((CFI_cdesc_t *)&source, lower));
                  // printf ("rank = %d\t", rank);
                  // printf ("extents = [ ");
                  for (int r = 0; r < rank; r++)
                    {
                      n_entries = n_entries * (upper[r] - lower[r] + 1);
                      // printf ("%ld ", extents[r]);
                    }

                  /**/
                  tr_subscripts = malloc (rank * sizeof (CFI_index_t));
                  tr_dim        = malloc (rank * sizeof (CFI_dim_t));
                  for (int i = 0; i < rank; i++)
                    {
                      CFI_index_t idx  = rank - i - 1;
                      tr_subscripts[i] = upper[idx];
                      tr_dim[i]        = source.dim[idx];
                      /* Normalise the subscripts to start counting the address
                       * from 0. */
                      tr_subscripts[i] -= tr_dim[i].lower_bound;
                    }
                  /* We assume column major order as that is how Fortran stores
                   * arrays. We
                   * calculate the memory address of the specified element via
                   * the canonical
                   * array dimension reduction map and multiplying by the memory
                   * stride. */
                  CFI_index_t index     = tr_subscripts[0] * tr_dim[0].sm;
                  CFI_index_t tmp_index = 1;
                  // printf ("tr_subscripts[0] = %ld\t\t\t\t\t\t\tindex =
                  // %ld\n",
                  //         tr_subscripts[0], index);
                  for (int i = 1; i < rank; i++)
                    {
                      tmp_index *= tr_subscripts[i] * tr_dim[i - 1].extent *
                                   tr_dim[i - 1].sm;
                      index += tmp_index;
                      // printf ("tr_subscripts[%d] = %ld\textent[%d] = "
                      //         "%ld\ttr_dim[%d].sm = %ld\ttmp_index = "
                      //         "%ld\tindex = %ld\n",
                      //         i, tr_subscripts[i], i - 1, tr_dim[i -
                      //         1].extent,
                      //         i - 1, tr_dim[i - 1].sm, tmp_index, index);
                    }
                  free (tr_subscripts);
                  free (tr_dim);
                  if (index - dif_addr != 0)
                    {
                      errno *= 2;
                      printf ("Error CFI_address is not being calculated "
                              "properly.\n");
                    }
                  printf ("errno = %ld\n", errno);
                  // if((CFI_index_t)add_dif != index){
                  //   printf ("dif_addr = %lu\tindex = %lu\n ", dif_addr,
                  //   index);
                  // }
                  /**/
                }
              else if (ind == CFI_ERROR_MEM_ALLOCATION)
                {
                  goto next_type;
                }
            }
        }
    next_type:;
    }

  // // This sets the value "val" at position "offset" for a CFI array "arr"
  // with element size "arr.elem_len" described by CFI_cdesc_t.
  // memcpy ((char *)arr.base_addr + offset * arr.elem_len,
  //         (void *)& val, arr.elem_len);
  // // This is the memory address of the l'th element of the array (C indices).
  // char address = (char *)test3.base_addr + l * test3.elem_len;
  // // This is the value of the l'th element of the array (C indices).
  // my_type value = *(my_type*)((char *)test3.base_addr + l * test3.elem_len);

  // if (type[i] == CFI_type_double)
  //   {
  //     size_t arr_len = 1;
  //     double val     = 0.;
  //     for (int r = 0; r < rank; r++)
  //       {
  //         arr_len *= test3.dim[r].extent;
  //       }
  //       printf("arr_len = %d\n", arr_len);
  //     for (size_t l = 0; l < arr_len; l++)
  //       {
  //         val++;
  //         /* Use memcpy */
  //         memcpy ((char *)test3.base_addr + l * test3.elem_len,
  //                 (void *)&val, test3.elem_len);
  //         printf ("val = %d, addr = %u\n",
  //                 (char *)test3.base_addr + l * test3.elem_len, (char
  //                 *)test3.base_addr + l * test3.elem_len);
  //         // printf(" %f ", *(double*)((char *)test3.base_addr + l *
  //         test3.elem_len));
  //         // test3.base_addr[l] = (void*)val;
  //       }
  //     printf ("\n\n");
  //   }

  // if (errno == 1)
  //   {
  //     return CFI_SUCCESS;
  //   }
  // else
  //   {
  //     return CFI_FAILURE;
  //   }
}
