#include "Pattern.h"
#include "base/Logger.h"

#include <iostream>
using std::cout;
using std::endl;


uint32 Pattern::msSeqPatternID	= 0	;


Pattern::Pattern (const Pattern *pPattern, Item *pItem) : ItemSet ()
{
//	LOGMSG (MAX_LEVEL, "Pattern::Patern (const Pattern *pPattern) - p [%p]\n", this);

	InitFields ();

	STLItemList_cit itPatternEnd = pPattern->GetEnd ();

	for (STLItemList_cit it = pPattern->GetBegin (); it != itPatternEnd; ++it)
		PushBack (static_cast<Item *>(*it));

	const TransactionList &rPatternTransactionList = pPattern->GetTransactionList ();

	TransactionList::STLTransactionList_cit itTransactionEnd = rPatternTransactionList.GetEnd ();

	for (TransactionList::STLTransactionList_cit it = rPatternTransactionList.GetBegin (); it != itTransactionEnd; ++it)
	{
		Transaction *pTransaction = static_cast<Transaction *>(*it);
		mNumTransactionsOfClassHsh [pTransaction->GetClassValue ()]++;
		mTransactionList.PushBack (pTransaction);
	}

	AddItem (pItem);

	mFrequence = mTransactionList.GetSize ();
}


Pattern::Pattern (Item *pItem) : ItemSet ()
{
//	LOGMSG (MAX_LEVEL, "Pattern::Patern (Item *pItem) - p [%p]\n", this);

	InitFields ();

	PushBack (pItem);

	const TransactionList *pItemTransactionList = pItem->GetProjectionTransactionList ();

	TransactionList::STLTransactionList_cit itTransactionEnd = pItemTransactionList->GetEnd ();

	for (TransactionList::STLTransactionList_cit it = pItemTransactionList->GetBegin (); it != itTransactionEnd; ++it)
	{
		Transaction *pTransaction = static_cast<Transaction *>(*it);
		mNumTransactionsOfClassHsh [pTransaction->GetClassValue ()]++;
		mTransactionList.PushBack (pTransaction);
	}

	mFrequence = mTransactionList.GetSize ();
}

Pattern::~Pattern ()
{
	mTransactionList.RemoveAll ();

	mClassCoverageHsh.clear ();
	mNumTransactionsOfClassHsh.clear ();

	RemoveAll ();
}

const uint32 Pattern::GetSeqPatternID ()
{
	uint32 patternID = msSeqPatternID;
	msSeqPatternID++;

	return patternID;
}

void Pattern::SetPatternID ()
{
	mPatternID = GetSeqPatternID ();
}

const uint32& Pattern::GetPatternID () const
{
	return mPatternID;
}

void Pattern::InitFields ()
{
	mPatternID	= 0	;
	mFrequence	= 0	;
	mSupport	= 0.0	;
	mGot		= false	;
}

void Pattern::AddItem (Item *pItem)
{
//	LOGMSG (MAX_LEVEL, "Pattern::AddItem () - begin\n");

	PushBack (pItem);

//	LOGMSG (MAX_LEVEL, "Pattern::AddItem () - merge lists\n");

	const TransactionList *pItemTransactionList = pItem->GetProjectionTransactionList ();

	TransactionList::STLTransactionList_it it	= mTransactionList.GetBegin ()	;
	TransactionList::STLTransactionList_it itEnd	= mTransactionList.GetEnd ()	;

	while (it != itEnd)
	{
		if (pItemTransactionList->FindByPtr (*it))
			++it;
		else
		{
			Transaction *pTransaction = static_cast<Transaction *>(*it);
			mNumTransactionsOfClassHsh [pTransaction->GetClassValue ()]--;

			it = mTransactionList.Erase (it);
			itEnd = mTransactionList.GetEnd ();
		}
	}
}

const uint32 Pattern::GetFrequence () const
{
	return mFrequence;
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

const uint32 Pattern::GetNumTransactionsOfClass (const string &class_name) const
{
	uint32 num_transactions = 0;

	hash_map<string, uint32>::const_iterator it = mNumTransactionsOfClassHsh.find (class_name);

	if (it != mNumTransactionsOfClassHsh.end ())
		num_transactions = it->second;

	return num_transactions;
}

void Pattern::SetGot (const bool &got)
{
	mGot = got;
}

const bool& Pattern::GetGot () const
{
	return mGot;
}

const float32 Pattern::GetSimilarity (Pattern *pPattern)
{
//	LOGMSG (MAX_LEVEL, "Pattern::GetSimilarity () - begin [%p]\n", this);

	float32 similarity = -1;

//	if (mPatternSimilarityHsh.find (pPattern->GetPatternID ()) != mPatternSimilarityHsh.end ())
//		similarity = mPatternSimilarityHsh [pPattern->GetPatternID ()];
//	else
//	{
		ItemList	totalItemList		;
		STLItemList_cit itEnd = GetEnd ()	;

		for (STLItemList_cit it = GetBegin (); it != itEnd; ++it)
		{
			Item *pItem = static_cast<Item *>(*it);

			//		LOGMSG (MAX_LEVEL, "Pattern::GetSimilarity () - key [%s]\n", pItem->GetValue ().c_str ());

			pItem->SetCount (1);
			totalItemList.PushBack (pItem);
		}

		itEnd = pPattern->GetEnd ();

		for (STLItemList_cit it = pPattern->GetBegin (); it != itEnd; ++it)
		{
			Item *pItem = static_cast<Item *>(*it);

			//		LOGMSG (MAX_LEVEL, "Pattern::GetSimilarity () - key [%s]\n", pItem->GetValue ().c_str ());

			if (totalItemList.FindByPtr (pItem))
				pItem->IncCount ();
			else
			{
				pItem->SetCount (1);
				totalItemList.PushBack (pItem);
			}
		}

		uint32 num	= 0	;
		uint32 den	= 0	;

		itEnd = totalItemList.GetEnd ();

		for (STLItemList_cit it = totalItemList.GetBegin (); it != itEnd; ++it)
		{
			const Item *pItem = static_cast<const Item *>(*it);

			den += pItem->GetCount ();

			if (pItem->GetCount () > 1)
				num += pItem->GetCount ();
		}

		totalItemList.RemoveAll ();

		similarity = (float32) num / den;

//		mPatternSimilarityHsh [pPattern->GetPatternID ()] = similarity;
//		pPattern->SetSimilarityHsh (mPatternID, similarity);
//	}

	return similarity;
}

void Pattern::IncClassCoverage (const string &class_name)
{
	mClassCoverageHsh [class_name]++;
}

const uint32& Pattern::GetClassCoverage (const string &class_name)
{
	return mClassCoverageHsh [class_name];
}

void Pattern::ResetClassCoverage ()
{
	hash_map<string, uint32>::iterator itEnd = mClassCoverageHsh.end ();

	for (hash_map<string, uint32>::iterator it = mClassCoverageHsh.begin (); it != itEnd; ++it)
		it->second = 0;
}

void Pattern::SetSimilarityHsh (const uint32 &rPatternID, const float32 &similarity)
{
	mPatternSimilarityHsh [rPatternID] = similarity;
}

void Pattern::Print () const
{
	LOGMSG (HIGH_LEVEL, "Pattern::Print () - support [%0.6f] - [%s]\n", mSupport, GetPrintableString ().c_str ());

//	cout << "suporte [" << mSupport << "], padrão [" << GetPrintableString () << "]" << endl;
}
