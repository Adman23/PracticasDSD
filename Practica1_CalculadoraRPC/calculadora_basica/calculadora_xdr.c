/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "calculadora.h"

bool_t
xdr_nums (XDR *xdrs, nums *objp)
{
	register int32_t *buf;

	 if (!xdr_array (xdrs, (char **)&objp->nums_val, (u_int *) &objp->nums_len, MAX,
		sizeof (int), (xdrproc_t) xdr_int))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_operands (XDR *xdrs, operands *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->op1))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->op2))
		 return FALSE;
	return TRUE;
}
