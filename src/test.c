// 
        // Initialize an array with values 1,2,3,...,length_of_array 
        // 
        // Notice: Any changes made to "array_variable" are reflected in 
        //         the calling code! Arrays are pass by reference! 
        // 
        // Notice: There is no return statement, but still the array is changed 
        //         and can be said to be "returned" to the calling function. 
        // 
        void 
        build_array( int array_variable[], int length_of_array ) 
        { 
            for (int i=0; i<length_of_array; i++) 
              { 
                array_variable[i] = i; 
              } 
        } 
 
 
        // 
        // Test code for passing an array by reference 
        // 
        int 
        main() 
        { 
          int values[50]; 
 
          printf("the value at location 7 starts as %d\n", values[7]); 
 
          build_array(values, 50); 
 
          printf("the value at location 7 is now %d\n", values[7]); 
 
          return 0; 
        } 
        