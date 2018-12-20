/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF5/releases.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef VOL_LINK_TEST_H
#define VOL_LINK_TEST_H

#include "vol_test.h"

int vol_link_test(void);

/*********************************************
 *                                           *
 *      VOL connector Link test defines      *
 *                                           *
 *********************************************/

#define HARD_LINK_TEST_LINK_NAME     "test_link"

#define H5L_SAME_LOC_TEST_DSET_SPACE_RANK 2
#define H5L_SAME_LOC_TEST_GROUP_NAME      "h5l_same_loc_test_group"
#if 0
#define H5L_SAME_LOC_TEST_LINK_NAME1      "h5l_same_loc_test_link1"
#endif
#define H5L_SAME_LOC_TEST_LINK_NAME2      "h5l_same_loc_test_link2"
#define H5L_SAME_LOC_TEST_DSET_NAME       "h5l_same_loc_test_dset"

#define SOFT_LINK_EXISTING_RELATIVE_TEST_DSET_SPACE_RANK 2
#define SOFT_LINK_EXISTING_RELATIVE_TEST_SUBGROUP_NAME   "soft_link_to_existing_relative_path_test"
#define SOFT_LINK_EXISTING_RELATIVE_TEST_DSET_NAME       "dset"
#define SOFT_LINK_EXISTING_RELATIVE_TEST_LINK_NAME       "soft_link_to_existing_relative_path"

#define SOFT_LINK_EXISTING_ABSOLUTE_TEST_SUBGROUP_NAME "soft_link_to_existing_absolute_path_test"
#define SOFT_LINK_EXISTING_ABSOLUTE_TEST_LINK_NAME     "soft_link_to_existing_absolute_path"

#define SOFT_LINK_DANGLING_RELATIVE_TEST_DSET_SPACE_RANK 2
#define SOFT_LINK_DANGLING_RELATIVE_TEST_SUBGROUP_NAME   "soft_link_dangling_relative_path_test"
#define SOFT_LINK_DANGLING_RELATIVE_TEST_DSET_NAME       "dset"
#define SOFT_LINK_DANGLING_RELATIVE_TEST_LINK_NAME       "soft_link_dangling_relative_path"

#define SOFT_LINK_DANGLING_ABSOLUTE_TEST_DSET_SPACE_RANK 2
#define SOFT_LINK_DANGLING_ABSOLUTE_TEST_SUBGROUP_NAME   "soft_link_dangling_absolute_path_test"
#define SOFT_LINK_DANGLING_ABSOLUTE_TEST_DSET_NAME       "dset"
#define SOFT_LINK_DANGLING_ABSOLUTE_TEST_LINK_NAME       "soft_link_dangling_absolute_path"

#define EXTERNAL_LINK_TEST_SUBGROUP_NAME "external_link_test"
#define EXTERNAL_LINK_TEST_FILE_NAME     "ext_link_file"
#define EXTERNAL_LINK_TEST_LINK_NAME     "ext_link"

#define EXTERNAL_LINK_TEST_DANGLING_DSET_SPACE_RANK 2
#define EXTERNAL_LINK_TEST_DANGLING_SUBGROUP_NAME   "external_link_dangling_test"
#define EXTERNAL_LINK_TEST_DANGLING_LINK_NAME       "dangling_ext_link"
#define EXTERNAL_LINK_TEST_DANGLING_DSET_NAME       "external_dataset"

#define UD_LINK_TEST_UDATA_MAX_SIZE 256
#define UD_LINK_TEST_LINK_NAME      "ud_link"

#define LINK_DELETE_TEST_DSET_SPACE_RANK     2
#define LINK_DELETE_TEST_EXTERNAL_LINK_NAME  "external_link"
#define LINK_DELETE_TEST_EXTERNAL_LINK_NAME2 "external_link2"
#define LINK_DELETE_TEST_SOFT_LINK_NAME      "soft_link"
#define LINK_DELETE_TEST_SOFT_LINK_NAME2     "soft_link2"
#define LINK_DELETE_TEST_SUBGROUP_NAME       "link_delete_test"
#define LINK_DELETE_TEST_DSET_NAME1          "link_delete_test_dset1"
#define LINK_DELETE_TEST_DSET_NAME2          "link_delete_test_dset2"

#define COPY_LINK_TEST_SOFT_LINK_TARGET_PATH "/" COPY_LINK_TEST_GROUP_NAME "/" COPY_LINK_TEST_DSET_NAME
#define COPY_LINK_TEST_HARD_LINK_COPY_NAME   "hard_link_to_dset_copy"
#define COPY_LINK_TEST_SOFT_LINK_COPY_NAME   "soft_link_to_dset_copy"
#define COPY_LINK_TEST_HARD_LINK_NAME        "hard_link_to_dset"
#define COPY_LINK_TEST_SOFT_LINK_NAME        "soft_link_to_dset"
#define COPY_LINK_TEST_GROUP_NAME            "link_copy_test_group"
#define COPY_LINK_TEST_DSET_NAME             "link_copy_test_dset"
#define COPY_LINK_TEST_DSET_SPACE_RANK       2

#define MOVE_LINK_TEST_SOFT_LINK_TARGET_PATH "/" MOVE_LINK_TEST_GROUP_NAME "/" MOVE_LINK_TEST_DSET_NAME
#define MOVE_LINK_TEST_HARD_LINK_NAME        "hard_link_to_dset"
#define MOVE_LINK_TEST_SOFT_LINK_NAME        "soft_link_to_dset"
#define MOVE_LINK_TEST_GROUP_NAME            "link_move_test_group"
#define MOVE_LINK_TEST_DSET_NAME             "link_move_test_dset"
#define MOVE_LINK_TEST_DSET_SPACE_RANK       2

#define GET_LINK_INFO_TEST_DSET_SPACE_RANK 2
#define GET_LINK_INFO_TEST_SUBGROUP_NAME   "get_link_info_test"
#define GET_LINK_INFO_TEST_SOFT_LINK_NAME  "soft_link"
#define GET_LINK_INFO_TEST_EXT_LINK_NAME   "ext_link"
#define GET_LINK_INFO_TEST_DSET_NAME       "get_link_info_dset"

#define GET_LINK_NAME_TEST_DSET_SPACE_RANK 2
#define GET_LINK_NAME_TEST_SUBGROUP_NAME   "get_link_name_test"
#define GET_LINK_NAME_TEST_DSET_NAME       "get_link_name_dset"

#define GET_LINK_VAL_TEST_SUBGROUP_NAME  "get_link_val_test"
#define GET_LINK_VAL_TEST_SOFT_LINK_NAME "soft_link"
#define GET_LINK_VAL_TEST_EXT_LINK_NAME  "ext_link"

#define LINK_ITER_TEST_DSET_SPACE_RANK 2
#define LINK_ITER_TEST_HARD_LINK_NAME  "link_iter_test_dset"
#define LINK_ITER_TEST_SOFT_LINK_NAME  "soft_link1"
#define LINK_ITER_TEST_EXT_LINK_NAME   "ext_link1"
#define LINK_ITER_TEST_SUBGROUP_NAME   "link_iter_test"
#define LINK_ITER_TEST_NUM_LINKS       3

#define LINK_ITER_TEST_0_LINKS_SUBGROUP_NAME "link_iter_test_0_links"

#define LINK_VISIT_TEST_NO_CYCLE_DSET_SPACE_RANK 2
#define LINK_VISIT_TEST_NO_CYCLE_DSET_NAME       "dset"
#define LINK_VISIT_TEST_NO_CYCLE_SUBGROUP_NAME   "link_visit_test_no_cycles"
#define LINK_VISIT_TEST_NO_CYCLE_SUBGROUP_NAME2  "link_visit_subgroup1"
#define LINK_VISIT_TEST_NO_CYCLE_SUBGROUP_NAME3  "link_visit_subgroup2"
#define LINK_VISIT_TEST_NO_CYCLE_LINK_NAME1      "hard_link1"
#define LINK_VISIT_TEST_NO_CYCLE_LINK_NAME2      "soft_link1"
#define LINK_VISIT_TEST_NO_CYCLE_LINK_NAME3      "ext_link1"
#define LINK_VISIT_TEST_NO_CYCLE_LINK_NAME4      "hard_link2"

#define LINK_VISIT_TEST_CYCLE_SUBGROUP_NAME  "link_visit_test_cycles"
#define LINK_VISIT_TEST_CYCLE_SUBGROUP_NAME2 "link_visit_subgroup1"
#define LINK_VISIT_TEST_CYCLE_SUBGROUP_NAME3 "link_visit_subgroup2"
#define LINK_VISIT_TEST_CYCLE_LINK_NAME1     "hard_link1"
#define LINK_VISIT_TEST_CYCLE_LINK_NAME2     "soft_link1"
#define LINK_VISIT_TEST_CYCLE_LINK_NAME3     "ext_link1"
#define LINK_VISIT_TEST_CYCLE_LINK_NAME4     "hard_link2"

#define LINK_VISIT_TEST_0_LINKS_SUBGROUP_NAME   "link_visit_test_0_links"
#define LINK_VISIT_TEST_0_LINKS_SUBGROUP_NAME2  "link_visit_test_0_links_subgroup1"
#define LINK_VISIT_TEST_0_LINKS_SUBGROUP_NAME3  "link_visit_test_0_links_subgroup2"

#endif
