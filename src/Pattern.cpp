#include "Pattern.h"
#include "TransactionList.h"
#include "base/Logger.h"

#include <iostream>
using std::cout;
using std::endl;


Pattern::Pattern () : ItemSet ()
{
	LOGMSG (MEDIUM_LEVEL, "Pattern::Patern () - p [%p]\n", this);

	mSupport		= 0.0				;
	mpTransactionList	= new TransactionList ()	;

	mGot			= false				;
}

Pattern::~Pattern ()
{
	mpTransactionList->RemoveAll ();

	delete mpTransactionList;

	RemoveAll ();
}

/*
const bool Pattern::operator< (const Object &rObject) const
{
	const Pattern &p = static_cast<const Pattern&>(rObject);

	uint32 max_size = GetSize () > p.GetSize () ? GetSize ():p.GetSize ();

	bool b = true;
	uint32 i = 0;

	while (i < max_size)
	{
		if (*(GetAt (i)) < *(p.GetAt (i)))
		{
			b = true;
			break;
		}
		else if (*(GetAt (i)) > *(p.GetAt (i)))
		{
			b = false;
			break;
		}

		i++;
	}

	if (i == max_size)
		b = GetSize () < p.GetSize ();

	if (b)
		LOGMSG (MAX_LEVEL, "Pattern::operator< () - [%s] < [%s]\n", GetPrintableString ().c_str (), static_cast<const Pattern&>(rObject).GetPrintableString ().c_str ());
	else
		LOGMSG (MAX_LEVEL, "Pattern::operator< () - [%s] >= [%s]\n", GetPrintableString ().c_str (), static_cast<const Pattern&>(rObject).GetPrintableString ().c_str ());

	return b;
}
*/

void Pattern::MakeTransactionList () const
{
	LOGMSG (MEDIUM_LEVEL, "Pattern::MakeTransactionList () - begin\n");

	const Item *pLessFrequentItem = NULL;
	STLItemList_cit it = GetBegin ();

	if (it != GetEnd ())
	{
		pLessFrequentItem = static_cast<const Item*>(*it);
		it++;
	}

	while (it != GetEnd ())
	{
		if (static_cast<const Item*>(*it)->GetFrequence () < pLessFrequentItem->GetFrequence ())
			pLessFrequentItem = static_cast<const Item*>(*it);

		it++;
	}

	LOGMSG (MEDIUM_LEVEL, "Pattern::MakeTransactionList () - less frequent item [%s]\n", pLessFrequentItem->GetValue ().c_str ());

	const TransactionList *pItemTransactionList = pLessFrequentItem->GetTransactionList ();

	for (uint32 i = 0; i < pItemTransactionList->GetSize (); i++)
		mpTransactionList->PushBack (static_cast<Transaction *>(pItemTransactionList->GetAt (i)));

	for (it = GetBegin (); it != GetEnd (); it++)
	{
		if (*it != pLessFrequentItem)
		{
			const Item *pItem = static_cast<const Item*>(*it);

			LOGMSG (MEDIUM_LEVEL, "Pattern::MakeTransactionList () - merge list with item [%s]\n", pItem->GetValue ().c_str ());

			pItemTransactionList = pItem->GetTransactionList ();

			STLItemList_it it2 = mpTransactionList->GetBegin ();

			while (it2 != mpTransactionList->GetEnd ())
			{
				if (pItemTransactionList->FindByPtr (*it2))
					it2++;
				else
					it2 = mpTransactionList->Erase (it2);
			}

			/*
			uint32 i = 0;

			while (i < mpTransactionList->GetSize ())
			{
				Transaction *pTransaction = static_cast<Transaction *>(mpTransactionList->GetAt (i));

				if (pItemTransactionList->FindByPtr (pTransaction))
					i++;
				else
					mpTransactionList->Remove (pTransaction);
			}
			*/

			/*
			for (uint32 i = 0; i < mpTransactionList->GetSize (); i++)
			{
				Transaction *pTransaction = static_cast<Transaction *>(mpTransactionList->GetAt (i));

				if (! pItemTransactionList->FindByPtr (pTransaction))
				{
					mpTransactionList->Remove (pTransaction);
					i--;
				}
			}
			*/
		}
	}
}

const uint64 Pattern::GetFrequence () const
{
	return mpTransactionList->GetSize ();
}

void Pattern::SetSupport (const float32 &support)
{
	mSupport = support;
}

const float32& Pattern::GetSupport () const
{
	return mSupport;
}

const TransactionList* Pattern::GetTransactionList () const
{
	return mpTransactionList;
}

void Pattern::SetGot (const bool &got)
{
	mGot = got;
}

const bool& Pattern::GetGot () const
{
	return mGot;
}

void Pattern::Print () const
{
	LOGMSG (LOW_LEVEL, "Pattern::Print () - support [%0.2f] - [%s]\n", mSupport, GetPrintableString ().c_str ());

//	cout << "suporte [" << mSupport << "], padrão [" << GetPrintableString () << "]" << endl;
}
