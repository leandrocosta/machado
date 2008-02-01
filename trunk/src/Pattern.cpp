#include "Pattern.h"
#include "base/Logger.h"

#include <iostream>
using std::cout;
using std::endl;


Pattern::Pattern (const Pattern *pPattern) : ItemSet ()
{
	LOGMSG (MEDIUM_LEVEL, "Pattern::Patern (const Pattern *pPattern) - p [%p]\n", this);

	InitFields ();

	STLItemList_cit itPatternEnd = pPattern->GetEnd ();

	for (STLItemList_cit it = pPattern->GetBegin (); it != itPatternEnd; it++)
		PushBack (static_cast<Item *>(*it));

	const TransactionList &rPatternTransactionList = pPattern->GetTransactionList ();

	TransactionList::STLTransactionList_cit itTransactionEnd = rPatternTransactionList.GetEnd ();

	for (TransactionList::STLTransactionList_cit it = rPatternTransactionList.GetBegin (); it != itTransactionEnd; it++)
		mTransactionList.PushBack (static_cast<Transaction *>(*it));
}


Pattern::Pattern (Item *pItem) : ItemSet ()
{
	LOGMSG (MEDIUM_LEVEL, "Pattern::Patern (Item *pItem) - p [%p]\n", this);

	InitFields ();

	PushBack (pItem);

	const TransactionList *pItemTransactionList = pItem->GetTransactionList ();

	TransactionList::STLTransactionList_cit itTransactionEnd = pItemTransactionList->GetEnd ();

	for (TransactionList::STLTransactionList_cit it = pItemTransactionList->GetBegin (); it != itTransactionEnd; it++)
		mTransactionList.PushBack (static_cast<Transaction *>(*it));
}



Pattern::Pattern () : ItemSet ()
{
	LOGMSG (MEDIUM_LEVEL, "Pattern::Patern () - p [%p]\n", this);

	InitFields ();
}

Pattern::~Pattern ()
{
	mTransactionList.RemoveAll ();

	RemoveAll ();
}

void Pattern::InitFields ()
{
	mSupport	= 0.0		;
	mGot		= false		;
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

void Pattern::AddItem (Item *pItem)
{
	LOGMSG (MEDIUM_LEVEL, "Pattern::AddItem () - begin\n");

	PushBack (pItem);

	LOGMSG (MEDIUM_LEVEL, "Pattern::AddItem () - merge lists\n");

	const TransactionList *pItemTransactionList = pItem->GetTransactionList ();

	TransactionList::STLTransactionList_it it	= mTransactionList.GetBegin ()	;
	TransactionList::STLTransactionList_it itEnd	= mTransactionList.GetEnd ()	;

	while (it != itEnd)
	{
		if (pItemTransactionList->FindByPtr (*it))
			it++;
		else
		{
			it = mTransactionList.Erase (it);
			itEnd = mTransactionList.GetEnd ();
		}
	}
}

void Pattern::MakeTransactionList ()
{
	LOGMSG (MEDIUM_LEVEL, "Pattern::MakeTransactionList () - begin\n");

	const Item *pLessFrequentItem = NULL;
	STLItemList_cit it	= GetBegin ()	;
	STLItemList_cit itEnd	= GetEnd ()	;

	if (it != itEnd)
	{
		pLessFrequentItem = static_cast<const Item*>(*it);
		it++;
	}

	while (it != itEnd)
	{
		if (static_cast<const Item*>(*it)->GetFrequence () < pLessFrequentItem->GetFrequence ())
			pLessFrequentItem = static_cast<const Item*>(*it);

		it++;
	}

	LOGMSG (MEDIUM_LEVEL, "Pattern::MakeTransactionList () - less frequent item [%s]\n", pLessFrequentItem->GetValue ().c_str ());

	const TransactionList *pItemTransactionList = pLessFrequentItem->GetTransactionList ();

	for (uint32 i = 0; i < pItemTransactionList->GetSize (); i++)
		mTransactionList.PushBack (static_cast<Transaction *>(pItemTransactionList->GetAt (i)));

	for (it = GetBegin (); it != itEnd; it++)
	{
		if (*it != pLessFrequentItem)
		{
			const Item *pItem = static_cast<const Item*>(*it);

			LOGMSG (MEDIUM_LEVEL, "Pattern::MakeTransactionList () - merge list with item [%s]\n", pItem->GetValue ().c_str ());

			pItemTransactionList = pItem->GetTransactionList ();

			TransactionList::STLTransactionList_it itTransaction	= mTransactionList.GetBegin ()	;
			TransactionList::STLTransactionList_it itTransactionEnd	= mTransactionList.GetEnd ()	;

			while (itTransaction != itTransactionEnd)
			{
				if (pItemTransactionList->FindByPtr (*itTransaction))
					itTransaction++;
				else
					itTransaction = mTransactionList.Erase (itTransaction);
			}

			/*
			uint32 i = 0;

			while (i < mTransactionList.GetSize ())
			{
				Transaction *pTransaction = static_cast<Transaction *>(mTransactionList.GetAt (i));

				if (pItemTransactionList->FindByPtr (pTransaction))
					i++;
				else
					mTransactionList.Remove (pTransaction);
			}
			*/

			/*
			for (uint32 i = 0; i < mTransactionList.GetSize (); i++)
			{
				Transaction *pTransaction = static_cast<Transaction *>(mTransactionList.GetAt (i));

				if (! pItemTransactionList->FindByPtr (pTransaction))
				{
					mTransactionList.Remove (pTransaction);
					i--;
				}
			}
			*/
		}
	}
}

void Pattern::MakeTransactionList (Item *pItem)
{
	LOGMSG (MEDIUM_LEVEL, "Pattern::MakeTransactionList (Item *pItem) - begin\n");

	const TransactionList *pItemTransactionList	= pItem->GetTransactionList ()		;
	TransactionList::STLTransactionList_cit itEnd	= pItemTransactionList->GetEnd ()	;

	for (TransactionList::STLTransactionList_cit it = pItemTransactionList->GetBegin (); it != itEnd; it++)
		mTransactionList.PushBack (static_cast<Transaction *>(*it));
}


const uint64 Pattern::GetFrequence () const
{
	return mTransactionList.GetSize ();
}

void Pattern::SetSupport (const float32 &support)
{
	mSupport = support;
}

const float32& Pattern::GetSupport () const
{
	return mSupport;
}

const TransactionList& Pattern::GetTransactionList () const
{
	return mTransactionList;
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
