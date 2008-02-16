#include "Pattern.h"
#include "Class.h"
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
		PushBack (*it);

	PushBack (pItem);

	const uint32 &rItemID = pItem->GetItemID ();

	const TransactionList &rPatternTransactionList = pPattern->GetTransactionList ();

	TransactionList::STLTransactionList_cit itTransactionEnd = rPatternTransactionList.GetEnd ();

	for (TransactionList::STLTransactionList_cit it = rPatternTransactionList.GetBegin (); it != itTransactionEnd; ++it)
	{
		Transaction *pTransaction = static_cast<Transaction *>(*it);

//		if (pTransaction->IsCoveredBy (pItem))
		if (pTransaction->IsCoveredByItem (rItemID))
		{
//			mNumTransactionsOfClassHsh.Set (pTransaction->GetClassValue (), mNumTransactionsOfClassHsh.Get (pTransaction->GetClassValue ()) + 1);
//			mNumTransactionsOfClassHsh [pTransaction->GetClassValue ()]++;
			mClassCoverageArray [pTransaction->GetClassID ()]++;
			mTransactionList.PushBack (pTransaction);
		}
	}

//	AddItem (pItem);

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
//		mNumTransactionsOfClassHsh.Set (pTransaction->GetClassValue (), mNumTransactionsOfClassHsh.Get (pTransaction->GetClassValue ()) + 1);
//		mNumTransactionsOfClassHsh [pTransaction->GetClassValue ()]++;
		mClassCoverageArray [pTransaction->GetClassID ()]++;
		mTransactionList.PushBack (pTransaction);
	}

	mFrequence = mTransactionList.GetSize ();
}

Pattern::~Pattern ()
{
	mTransactionList.RemoveAll ();

//	mNumTransactionsOfClassHsh.Clear ();
//	mClassCoverageHsh.Clear ();
//	mNumTransactionsOfClassHsh.clear ();
//	mClassCoverageHsh.clear ();

	delete[] mClassCoverageArray;

	RemoveAll ();
}

const uint32 Pattern::GetSeqPatternID ()
{
	uint32 patternID = msSeqPatternID++;

	return patternID;
}

const uint32 Pattern::GetMaxPatternID ()
{
	return msSeqPatternID - 1;
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

	const uint32 num_classes = Class::GetMaxClassID () + 1;

	mClassCoverageArray = new uint32 [num_classes];

	for (uint32 classID = 0; classID < num_classes; classID++)
		mClassCoverageArray [classID] = 0;
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

/*
const uint32 Pattern::GetNumTransactionsOfClass (const string &class_name) const
{
//	return mNumTransactionsOfClassHsh.Get (class_name);

	uint32 num_transactions = 0;

	hash_map<string, uint32>::const_iterator it = mNumTransactionsOfClassHsh.find (class_name);

	if (it != mNumTransactionsOfClassHsh.end ())
		num_transactions = it->second;

	return num_transactions;
}
*/

const uint32 Pattern::GetNumTransactionsOfClass (const uint32 &classID) const
{
	return mClassCoverageArray [classID];
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

/*
void Pattern::IncClassCoverage (const string &class_name)
{
//	mClassCoverageHsh.Set (class_name, mClassCoverageHsh.Get (class_name) + 1);
	mClassCoverageHsh [class_name]++;
}

const uint32 Pattern::GetClassCoverage (const string &class_name)
{
//	return mClassCoverageHsh.Get (class_name);
	return mClassCoverageHsh [class_name];
}

void Pattern::ResetClassCoverage ()
{
//
	hash_map<string, uint32>::iterator itEnd = mClassCoverageHsh.end ();

	for (hash_map<string, uint32>::iterator it = mClassCoverageHsh.begin (); it != itEnd; ++it)
		it->second = 0;
//

//	mClassCoverageHsh.Clear ();
	mClassCoverageHsh.clear ();
}
*/

/*
void Pattern::SetSimilarityHsh (const uint32 &rPatternID, const float32 &similarity)
{
	mPatternSimilarityHsh.Set (rPatternID, similarity);
}
*/

void Pattern::Print () const
{
	LOGMSG (HIGH_LEVEL, "Pattern::Print () - support [%0.6f] - [%s]\n", mSupport, GetPrintableString ().c_str ());

//	cout << "suporte [" << mSupport << "], padrão [" << GetPrintableString () << "]" << endl;
}
