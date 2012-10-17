/*
 * The contents of this file are subject to the MonetDB Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.monetdb.org/Legal/MonetDBLicense
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is the MonetDB Database System.
 *
 * The Initial Developer of the Original Code is CWI.
 * Portions created by CWI are Copyright (C) 1997-July 2008 CWI.
 * Copyright August 2008-2012 MonetDB B.V.
 * All Rights Reserved.
 */

/* This contains algebra functions used for RDF store only */

#include "monetdb_config.h"
#include "rdf.h"
#include "algebra.h"
#include <gdk.h>

str
RDFleftfetchjoin_sorted(bat *result, bat *lid, bat *rid)
{
	BAT *left, *right, *bn = NULL;

	if ((left = BATdescriptor(*lid)) == NULL) {
		throw(MAL, "rdf.leftfetchjoin_sorted", RUNTIME_OBJECT_MISSING);
	}
	if ((right = BATdescriptor(*rid)) == NULL) {
		BBPreleaseref(left->batCacheid);
		throw(MAL, "rdf.leftfetchjoin_sorted", RUNTIME_OBJECT_MISSING);
	}
	bn = BATleftfetchjoin(left, right, BUN_NONE);
	BBPreleaseref(left->batCacheid);
	BBPreleaseref(right->batCacheid);
	if (bn == NULL)
		throw(MAL, "rdf.leftfetchjoin_sorted", GDK_EXCEPTION);

	bn->tsorted = TRUE;

	if (!(bn->batDirty & 2))
		bn = BATsetaccess(bn, BAT_READ);
	*result = bn->batCacheid;
	BBPkeepref(*result);
	return MAL_SUCCEED;
}
