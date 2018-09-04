#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "arrayIntersection.h"
#include "idtoword.h"


//O(n + m)
void intersect(tuple *a, int alen, rhymeTuple *b, int blen)
{
	int ai = 0, bi = 0;


	while (ai < alen && bi < blen)
	{
		if (a[ai].wordid == b[bi].wordid)
		{//intersection!
			a[ai].rhymeLevel = b[bi].rhymeStr;
			b[bi].relatedness = a[ai].relatedness;

			++ai;
			++bi;
		}
		else if (a[ai].wordid > b[bi].wordid)
		{
			++bi;
		}
		else
			++ai;
	}
}
