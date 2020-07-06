#include "TestsCiclad.h"

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

void test_first() {
  const char* source = "../testdb_gen2.txt";
  char* const sourcePtr = _strdup(source);
  const uint32_t result = start_ciclad(sourcePtr, 1000, 1001);
  CU_ASSERT_EQUAL(result, 78765);
}

void test_second() {
  const char* source = "../testdb_gen.txt";
  char* const sourcePtr = _strdup(source);
  const uint32_t result = start_ciclad(sourcePtr, 1000, 10001);
  CU_ASSERT_EQUAL(result, 78765);
}

int test_output() {
  CU_pSuite pSuite = NULL;

  /* initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  //CU_cleanup_registry();

  /* add a suite to the registry */
  pSuite = CU_add_suite("ciclad_test_suite", init_suite, clean_suite);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* add the tests to the suite */
  if ((NULL == CU_add_test(pSuite, "ciclad_test_1", test_first)) 
    || (NULL == CU_add_test(pSuite, "ciclad_test_2", test_second)))
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  // Run all tests using the basic interface
  /*
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  printf("\n");
  CU_basic_show_failures(CU_get_failure_list());
  printf("\n\n");
  */
  
  // LES modes interactifs
  // Run all tests using the automated interface
  const char* testOutput = "./cicladTests-output.xml";
  CU_set_output_filename(testOutput);
  CU_automated_enable_junit_xml(1);
  CU_automated_run_tests();
  CU_list_tests_to_file();

     // Run all tests using the console interface
  //   CU_console_run_tests();
  
  /* Clean up registry and return */
  CU_cleanup_registry();
  return CU_get_error();
}