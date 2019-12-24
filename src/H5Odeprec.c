/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF5/releases.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*-------------------------------------------------------------------------
 *
 * Purpose:	Deprecated functions from the H5O interface.  These
 *              functions are here for compatibility purposes and may be
 *              removed in the future.  Applications should switch to the
 *              newer APIs.
 *
 *-------------------------------------------------------------------------
 */

/****************/
/* Module Setup */
/****************/

#include "H5Omodule.h"          /* This source code file is part of the H5O module */


/***********/
/* Headers */
/***********/
#include "H5private.h"          /* Generic Functions                        */
#include "H5CXprivate.h"        /* API Contexts                             */
#include "H5Eprivate.h"         /* Error handling                           */
#include "H5Iprivate.h"         /* IDs                                      */
#include "H5Opkg.h"             /* Object headers                           */

#include "H5VLnative_private.h" /* Native VOL connector                     */

#ifndef H5_NO_DEPRECATED_SYMBOLS

/****************/
/* Local Macros */
/****************/


/******************/
/* Local Typedefs */
/******************/

/* Adapter for using deprecated H5Ovisit1 callbacks with the VOL */
typedef struct H5O_visit1_adapter_t {
    H5O_iterate1_t   real_op;           /* Application callback to invoke */
    void            *real_op_data;      /* Application op_data */
} H5O_visit1_adapter_t;


/********************/
/* Package Typedefs */
/********************/


/********************/
/* Local Prototypes */
/********************/


/*********************/
/* Package Variables */
/*********************/


/*****************************/
/* Library Private Variables */
/*****************************/


/*******************/
/* Local Variables */
/*******************/




/*-------------------------------------------------------------------------
 * Function:    H5Oget_info1
 *
 * Purpose:     Retrieve information about an object.
 *
 * Return:      Success:    Non-negative
 *              Failure:    Negative
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Oget_info1(hid_t loc_id, H5O_info1_t *oinfo)
{
    H5VL_object_t      *vol_obj         = NULL;         /* Object token of loc_id */
    H5VL_loc_params_t   loc_params;
    herr_t      ret_value = SUCCEED;    /* Return value */

    FUNC_ENTER_API(FAIL)
    H5TRACE2("e", "i*x", loc_id, oinfo);

    /* Check args */
    if(!oinfo)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "oinfo parameter cannot be NULL")

    /* Set location struct fields */
    loc_params.type         = H5VL_OBJECT_BY_SELF;
    loc_params.obj_type     = H5I_get_type(loc_id);

    /* Get the location object */
    if(NULL == (vol_obj = H5VL_vol_object(loc_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid location identifier")

    /* Retrieve the object's information */
    if(H5O__get_info_old(vol_obj, &loc_params, oinfo, H5O_INFO_ALL) < 0)
        HGOTO_ERROR(H5E_OHDR, H5E_CANTGET, FAIL, "can't get deprecated info for object")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Oget_info1() */


/*-------------------------------------------------------------------------
 * Function:    H5Oget_info_by_name1
 *
 * Purpose:     Retrieve information about an object.
 *
 * Return:      Success:    Non-negative
 *              Failure:    Negative
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Oget_info_by_name1(hid_t loc_id, const char *name, H5O_info1_t *oinfo, hid_t lapl_id)
{
    H5VL_object_t    *vol_obj = NULL;        /* object token of loc_id */
    H5VL_loc_params_t loc_params;
    herr_t      ret_value = SUCCEED;    /* Return value */

    FUNC_ENTER_API(FAIL)
    H5TRACE4("e", "i*s*xi", loc_id, name, oinfo, lapl_id);

    /* Check args */
    if(!name)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "name parameter cannot be NULL")
    if(!*name)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "name parameter cannot be an empty string")
    if(!oinfo)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "oinfo parameter cannot be NULL")

    /* Verify access property list and set up collective metadata if appropriate */
    if(H5CX_set_apl(&lapl_id, H5P_CLS_LACC, loc_id, FALSE) < 0)
        HGOTO_ERROR(H5E_OHDR, H5E_CANTSET, FAIL, "can't set access property list info")

    /* Fill out location struct */
    loc_params.type                         = H5VL_OBJECT_BY_NAME;
    loc_params.loc_data.loc_by_name.name    = name;
    loc_params.loc_data.loc_by_name.lapl_id = lapl_id;
    loc_params.obj_type                     = H5I_get_type(loc_id);

    /* Get the location object */
    if(NULL == (vol_obj = H5VL_vol_object(loc_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid location identifier")

    /* Retrieve the object's information */
    if(H5O__get_info_old(vol_obj, &loc_params, oinfo, H5O_INFO_ALL) < 0)
        HGOTO_ERROR(H5E_OHDR, H5E_CANTGET, FAIL, "can't get deprecated info for object")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Oget_info_by_name1() */


/*-------------------------------------------------------------------------
 * Function:    H5Oget_info_by_idx1
 *
 * Purpose:     Retrieve information about an object, according to the order
 *              of an index.
 *
 * Return:      Success:    Non-negative
 *              Failure:    Negative
 *
 * Programmer:  Quincey Koziol
 *              November 26 2006
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Oget_info_by_idx1(hid_t loc_id, const char *group_name, H5_index_t idx_type,
    H5_iter_order_t order, hsize_t n, H5O_info1_t *oinfo, hid_t lapl_id)
{
    H5VL_object_t    *vol_obj = NULL;        /* object token of loc_id */
    H5VL_loc_params_t loc_params;
    herr_t      ret_value = SUCCEED;    /* Return value */

    FUNC_ENTER_API(FAIL)
    H5TRACE7("e", "i*sIiIoh*xi", loc_id, group_name, idx_type, order, n, oinfo,
             lapl_id);

    /* Check args */
    if(!group_name || !*group_name)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no name specified")
    if(idx_type <= H5_INDEX_UNKNOWN || idx_type >= H5_INDEX_N)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "invalid index type specified")
    if(order <= H5_ITER_UNKNOWN || order >= H5_ITER_N)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "invalid iteration order specified")
    if(!oinfo)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no info struct")

    /* Verify access property list and set up collective metadata if appropriate */
    if(H5CX_set_apl(&lapl_id, H5P_CLS_LACC, loc_id, FALSE) < 0)
        HGOTO_ERROR(H5E_OHDR, H5E_CANTSET, FAIL, "can't set access property list info")

    loc_params.type = H5VL_OBJECT_BY_IDX;
    loc_params.loc_data.loc_by_idx.name = group_name;
    loc_params.loc_data.loc_by_idx.idx_type = idx_type;
    loc_params.loc_data.loc_by_idx.order = order;
    loc_params.loc_data.loc_by_idx.n = n;
    loc_params.loc_data.loc_by_idx.lapl_id = lapl_id;
    loc_params.obj_type = H5I_get_type(loc_id);

    /* Get the location object */
    if(NULL == (vol_obj = H5VL_vol_object(loc_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid location identifier")

    /* Retrieve the object's information */
    if(H5O__get_info_old(vol_obj, &loc_params, oinfo, H5O_INFO_ALL) < 0)
        HGOTO_ERROR(H5E_OHDR, H5E_CANTGET, FAIL, "can't get deprecated info for object")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Oget_info_by_idx1() */


/*-------------------------------------------------------------------------
 * Function:    H5O__iterate1_adapter
 *
 * Purpose:     Retrieve information about an object, according to the order
 *              of an index.
 *
 * Return:      Success:    Non-negative
 *              Failure:    Negative
 *
 * Programmer:  Quincey Koziol
 *              November 26 2006
 *
 *-------------------------------------------------------------------------
 */
static herr_t
H5O__iterate1_adapter(hid_t obj_id, const char *name, const H5O_info2_t *oinfo2,
    void *op_data)
{
    H5O_visit1_adapter_t *shim_data = (H5O_visit1_adapter_t *)op_data;
    H5O_info1_t oinfo;          /* Deprecated object info struct */
    H5VL_object_t *vol_obj;     /* Object token of obj_id */
    H5VL_loc_params_t loc_params; /* Location parameters for VOL callback */
    H5O_native_info_t nat_info; /* Native object info */
    herr_t ret_value = H5_ITER_CONT; /* Return value */

    FUNC_ENTER_STATIC

    /* Sanity check */
    HDassert(oinfo2);
    HDassert(op_data);

    /* Reset the deprecated object info struct */
    HDmemset(&oinfo, 0, sizeof(H5O_info1_t));

    /* Copy the data model info into the deprecated struct */
    oinfo.fileno    = oinfo2->fileno;
    HDmemcpy(&oinfo.addr, &oinfo2->token, sizeof(oinfo.addr));
    oinfo.type      = oinfo2->type;
    oinfo.rc        = oinfo2->rc;
    oinfo.atime     = oinfo2->atime;
    oinfo.mtime     = oinfo2->mtime;
    oinfo.ctime     = oinfo2->ctime;
    oinfo.btime     = oinfo2->btime;
    oinfo.num_attrs = oinfo2->num_attrs;

    /* Retrieve the 'native' object info fields for this object */
    loc_params.type         = H5VL_OBJECT_BY_SELF;
    loc_params.obj_type     = H5I_get_type(obj_id);

    /* Get the location object */
    if(NULL == (vol_obj = H5VL_vol_object(obj_id)))
        HGOTO_ERROR(H5E_OHDR, H5E_BADTYPE, H5_ITER_ERROR, "invalid location identifier")

    /* Retrieve the object's native information */
    if(H5VL_object_optional(vol_obj, H5VL_NATIVE_OBJECT_GET_NATIVE_INFO, H5P_DATASET_XFER_DEFAULT, H5_REQUEST_NULL, &loc_params, &nat_info, H5O_INFO_ALL) < 0)
        HGOTO_ERROR(H5E_OHDR, H5E_CANTGET, H5_ITER_ERROR, "can't get native info for object")

    /* Set the native fields */
    HDmemcpy(&(oinfo.hdr), &(nat_info.hdr), sizeof(H5O_hdr_info_t));
    HDmemcpy(&(oinfo.meta_size.obj),  &(nat_info.meta_size.obj),  sizeof(H5_ih_info_t));
    HDmemcpy(&(oinfo.meta_size.attr), &(nat_info.meta_size.attr), sizeof(H5_ih_info_t));

    /* Invoke the application callback */
    ret_value = (shim_data->real_op)(obj_id, name, &oinfo, shim_data->real_op_data);

done:
    FUNC_LEAVE_NOAPI(ret_value);
} /* end H5O__iterate1_adapter() */


/*-------------------------------------------------------------------------
 * Function:	H5Ovisit1
 *
 * Purpose:     Recursively visit an object and all the objects reachable
 *              from it.  If the starting object is a group, all the objects
 *              linked to from that group will be visited.  Links within
 *              each group are visited according to the order within the
 *              specified index (unless the specified index does not exist for
 *              a particular group, then the "name" index is used).
 *
 *              NOTE: Soft links and user-defined links are ignored during
 *              this operation.
 *
 *              NOTE: Each _object_ reachable from the initial group will only
 *              be visited once.  If multiple hard links point to the same
 *              object, the first link to the object's path (according to the
 *              iteration index and iteration order given) will be used to in
 *              the callback about the object.
 *
 * Return:      Success:    The return value of the first operator that
 *				returns non-zero, or zero if all members were
 *				processed with no operator returning non-zero.
 *
 *              Failure:    Negative if something goes wrong within the
 *				library, or the negative value returned by one
 *				of the operators.
 *
 * Programmer:	Quincey Koziol
 *              November 25 2007
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Ovisit1(hid_t obj_id, H5_index_t idx_type, H5_iter_order_t order,
    H5O_iterate1_t op, void *op_data)
{
    H5VL_object_t      *vol_obj     = NULL;     /* Object token of loc_id */
    H5VL_loc_params_t   loc_params;
    H5O_visit1_adapter_t shim_data;             /* Adapter for passing app callback & user data */
    herr_t              ret_value;              /* Return value */

    FUNC_ENTER_API(FAIL)
    H5TRACE5("e", "iIiIox*x", obj_id, idx_type, order, op, op_data);

    /* Check args */
    if(idx_type <= H5_INDEX_UNKNOWN || idx_type >= H5_INDEX_N)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "invalid index type specified")
    if(order <= H5_ITER_UNKNOWN || order >= H5_ITER_N)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "invalid iteration order specified")
    if(!op)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no callback operator specified")

    /* Get the location object */
    if(NULL == (vol_obj = H5VL_vol_object(obj_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid location identifier")

    /* Set location parameters */
    loc_params.type         = H5VL_OBJECT_BY_SELF;
    loc_params.obj_type     = H5I_get_type(obj_id);

    /* Set up adapter */
    shim_data.real_op = op;
    shim_data.real_op_data = op_data;

    /* Visit the objects */
    if((ret_value = H5VL_object_specific(vol_obj, &loc_params, H5VL_OBJECT_VISIT, H5P_DATASET_XFER_DEFAULT, H5_REQUEST_NULL, (int)idx_type, (int)order, H5O__iterate1_adapter, (void *)&shim_data, H5O_INFO_ALL)) < 0)
        HGOTO_ERROR(H5E_OHDR, H5E_BADITER, FAIL, "object visitation failed")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Ovisit1() */


/*-------------------------------------------------------------------------
 * Function:    H5Ovisit_by_name1
 *
 * Purpose:     Recursively visit an object and all the objects reachable
 *              from it.  If the starting object is a group, all the objects
 *              linked to from that group will be visited.  Links within
 *              each group are visited according to the order within the
 *              specified index (unless the specified index does not exist for
 *              a particular group, then the "name" index is used).
 *
 *              NOTE: Soft links and user-defined links are ignored during
 *              this operation.
 *
 *              NOTE: Each _object_ reachable from the initial group will only
 *              be visited once.  If multiple hard links point to the same
 *              object, the first link to the object's path (according to the
 *              iteration index and iteration order given) will be used to in
 *              the callback about the object.
 *
 * Return:      Success:    The return value of the first operator that
 *				returns non-zero, or zero if all members were
 *				processed with no operator returning non-zero.
 *
 *              Failure:    Negative if something goes wrong within the
 *				library, or the negative value returned by one
 *				of the operators.
 *
 * Programmer:	Quincey Koziol
 *              November 24 2007
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Ovisit_by_name1(hid_t loc_id, const char *obj_name, H5_index_t idx_type,
    H5_iter_order_t order, H5O_iterate1_t op, void *op_data, hid_t lapl_id)
{
    H5VL_object_t       *vol_obj    = NULL;     /* Object token of loc_id */
    H5VL_loc_params_t   loc_params;
    H5O_visit1_adapter_t shim_data;             /* Adapter for passing app callback & user data */
    herr_t              ret_value;              /* Return value */

    FUNC_ENTER_API(FAIL)
    H5TRACE7("e", "i*sIiIox*xi", loc_id, obj_name, idx_type, order, op, op_data,
             lapl_id);

    /* Check args */
    if(!obj_name)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "obj_name parameter cannot be NULL")
    if(!*obj_name)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "obj_name parameter cannot be an empty string")
    if(idx_type <= H5_INDEX_UNKNOWN || idx_type >= H5_INDEX_N)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "invalid index type specified")
    if(order <= H5_ITER_UNKNOWN || order >= H5_ITER_N)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "invalid iteration order specified")
    if(!op)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no callback operator specified")

    /* Verify access property list and set up collective metadata if appropriate */
    if(H5CX_set_apl(&lapl_id, H5P_CLS_LACC, loc_id, FALSE) < 0)
        HGOTO_ERROR(H5E_OHDR, H5E_CANTSET, FAIL, "can't set access property list info")

    /* Get the location object */
    if(NULL == (vol_obj = H5VL_vol_object(loc_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid location identifier")

    /* Set location parameters */
    loc_params.type                         = H5VL_OBJECT_BY_NAME;
    loc_params.loc_data.loc_by_name.name    = obj_name;
    loc_params.loc_data.loc_by_name.lapl_id = lapl_id;
    loc_params.obj_type                     = H5I_get_type(loc_id);

    /* Set up adapter */
    shim_data.real_op = op;
    shim_data.real_op_data = op_data;

    /* Visit the objects */
    if((ret_value = H5VL_object_specific(vol_obj, &loc_params, H5VL_OBJECT_VISIT, H5P_DATASET_XFER_DEFAULT, H5_REQUEST_NULL, (int)idx_type, (int)order, H5O__iterate1_adapter, (void *)&shim_data, H5O_INFO_ALL)) < 0)
        HGOTO_ERROR(H5E_OHDR, H5E_BADITER, FAIL, "object visitation failed")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Ovisit_by_name1() */


/*-------------------------------------------------------------------------
 * Function:    H5Ovisit2
 *
 * Purpose:     Recursively visit an object and all the objects reachable
 *              from it.  If the starting object is a group, all the objects
 *              linked to from that group will be visited.  Links within
 *              each group are visited according to the order within the
 *              specified index (unless the specified index does not exist for
 *              a particular group, then the "name" index is used).
 *
 *              NOTE: Soft links and user-defined links are ignored during
 *              this operation.
 *
 *              NOTE: Each _object_ reachable from the initial group will only
 *              be visited once.  If multiple hard links point to the same
 *              object, the first link to the object's path (according to the
 *              iteration index and iteration order given) will be used to in
 *              the callback about the object.
 *
 *              NOTE: Add a a parameter "fields" to indicate selection of
 *              object info to be retrieved to the callback "op".
 *
 * Return:      Success:    The return value of the first operator that
 *                          returns non-zero, or zero if all members were
 *                          processed with no operator returning non-zero.
 *
 *              Failure:    Negative if something goes wrong within the
 *                          library, or the negative value returned by one
 *                          of the operators.
 *
 * Programmer:	Quincey Koziol
 *              November 25 2007
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Ovisit2(hid_t obj_id, H5_index_t idx_type, H5_iter_order_t order,
    H5O_iterate1_t op, void *op_data, unsigned fields)
{
    H5VL_object_t *vol_obj;             /* Object token of loc_id */
    H5VL_loc_params_t   loc_params;
    H5O_visit1_adapter_t shim_data;             /* Adapter for passing app callback & user data */
    herr_t              ret_value;              /* Return value */

    FUNC_ENTER_API(FAIL)
    H5TRACE6("e", "iIiIox*xIu", obj_id, idx_type, order, op, op_data, fields);

    /* Check args */
    if(idx_type <= H5_INDEX_UNKNOWN || idx_type >= H5_INDEX_N)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "invalid index type specified")
    if(order <= H5_ITER_UNKNOWN || order >= H5_ITER_N)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "invalid iteration order specified")
    if(!op)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no callback operator specified")
    if(fields & ~H5O_INFO_ALL)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "invalid fields")

    /* Get the location object */
    if(NULL == (vol_obj = H5VL_vol_object(obj_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid location identifier")

    /* Set location parameters */
    loc_params.type         = H5VL_OBJECT_BY_SELF;
    loc_params.obj_type     = H5I_get_type(obj_id);

    /* Set up adapter */
    shim_data.real_op = op;
    shim_data.real_op_data = op_data;

    /* Visit the objects */
    if((ret_value = H5VL_object_specific(vol_obj, &loc_params, H5VL_OBJECT_VISIT, H5P_DATASET_XFER_DEFAULT, H5_REQUEST_NULL, (int)idx_type, (int)order, H5O__iterate1_adapter, (void *)&shim_data, fields)) < 0)
        HGOTO_ERROR(H5E_OHDR, H5E_BADITER, FAIL, "object iteration failed")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Ovisit2() */


/*-------------------------------------------------------------------------
 * Function:    H5Ovisit_by_name2
 *
 * Purpose:     Recursively visit an object and all the objects reachable
 *              from it.  If the starting object is a group, all the objects
 *              linked to from that group will be visited.  Links within
 *              each group are visited according to the order within the
 *              specified index (unless the specified index does not exist for
 *              a particular group, then the "name" index is used).
 *
 *              NOTE: Soft links and user-defined links are ignored during
 *              this operation.
 *
 *              NOTE: Each _object_ reachable from the initial group will only
 *              be visited once.  If multiple hard links point to the same
 *              object, the first link to the object's path (according to the
 *              iteration index and iteration order given) will be used to in
 *              the callback about the object.
 *
 *              NOTE: Add a a parameter "fields" to indicate selection of
 *              object info to be retrieved to the callback "op".
 *
 * Return:      Success:    The return value of the first operator that
 *                          returns non-zero, or zero if all members were
 *                          processed with no operator returning non-zero.
 *
 *              Failure:    Negative if something goes wrong within the
 *                          library, or the negative value returned by one
 *                          of the operators.
 *
 * Programmer:	Quincey Koziol
 *              November 24 2007
 *
 *-------------------------------------------------------------------------
 */
herr_t
H5Ovisit_by_name2(hid_t loc_id, const char *obj_name, H5_index_t idx_type,
    H5_iter_order_t order, H5O_iterate1_t op, void *op_data, unsigned fields, hid_t lapl_id)
{
    H5VL_object_t *vol_obj;             /* Object token of loc_id */
    H5VL_loc_params_t   loc_params;
    H5O_visit1_adapter_t shim_data;             /* Adapter for passing app callback & user data */
    herr_t              ret_value;              /* Return value */

    FUNC_ENTER_API(FAIL)

    /* Check args */
    if(!obj_name)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "obj_name parameter cannot be NULL")
    if(!*obj_name)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "obj_name parameter cannot be an empty string")
    if(idx_type <= H5_INDEX_UNKNOWN || idx_type >= H5_INDEX_N)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "invalid index type specified")
    if(order <= H5_ITER_UNKNOWN || order >= H5_ITER_N)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "invalid iteration order specified")
    if(!op)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "no callback operator specified")
    if(fields & ~H5O_INFO_ALL)
        HGOTO_ERROR(H5E_ARGS, H5E_BADVALUE, FAIL, "invalid fields")

    /* Verify access property list and set up collective metadata if appropriate */
    if(H5CX_set_apl(&lapl_id, H5P_CLS_LACC, loc_id, FALSE) < 0)
        HGOTO_ERROR(H5E_OHDR, H5E_CANTSET, FAIL, "can't set access property list info")

    /* Get the location object */
    if(NULL == (vol_obj = H5VL_vol_object(loc_id)))
        HGOTO_ERROR(H5E_ARGS, H5E_BADTYPE, FAIL, "invalid location identifier")

    /* Set location parameters */
    loc_params.type                         = H5VL_OBJECT_BY_NAME;
    loc_params.loc_data.loc_by_name.name    = obj_name;
    loc_params.loc_data.loc_by_name.lapl_id = lapl_id;
    loc_params.obj_type                     = H5I_get_type(loc_id);

    /* Set up adapter */
    shim_data.real_op = op;
    shim_data.real_op_data = op_data;

    /* Visit the objects */
    if((ret_value = H5VL_object_specific(vol_obj, &loc_params, H5VL_OBJECT_VISIT, H5P_DATASET_XFER_DEFAULT, H5_REQUEST_NULL, (int)idx_type, (int)order, H5O__iterate1_adapter, (void *)&shim_data, fields)) < 0)
        HGOTO_ERROR(H5E_OHDR, H5E_BADITER, FAIL, "object iteration failed")

done:
    FUNC_LEAVE_API(ret_value)
} /* end H5Ovisit_by_name2() */

#endif /* H5_NO_DEPRECATED_SYMBOLS */

