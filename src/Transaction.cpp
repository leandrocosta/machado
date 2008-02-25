#include "Transaction.h"
#include "Class.h"
#include "ItemList.h"
#include "Pattern.h"
#include "PatternList.h"
#include "DataBaseException.h"
#include "base/Rand.h"
#include "base/Logger.h"

#include <iostream>
using std::cout;
using std::endl;


uint32 Transaction::msSeqTransactionID	= 0	;


Transaction::Transaction (Class *pClass) : ItemSet (), mTransactionID (GetSeqTransactionID ()), mpClass (pClass)
{
	mItemArray = NULL;
}

Transaction::~Transaction ()
{
	if (mItemArray)
		delete[] mItemArray;

	RemoveAll ();
}

const uint32 Transaction::GetSeqTransactionID ()
{
	uint32 transactionID = msSeqTransactionID++;

	return transactionID;
}

const uint32 Transaction::GetMaxTransactionID ()
{
	return msSeqTransactionID - 1;
}

const uint32 Transaction::GetNumTransactions ()
{
	return msSeqTransactionID;
}

const uint32& Transaction::GetTransactionID () const
{
	return mTransactionID;
}

Class* Transaction::GetClass ()
{
	return mpClass;
}

const uint32& Transaction::GetClassID () const
{
	return mpClass->GetClassID ();
}

const string& Transaction::GetClassValue () const
{
	return mpClass->GetValue ();
}

const bool Transaction::IsCoveredBy (const Pattern *pPattern) const
{
	bool bRet = true;

	ItemList::STLItemList_cit itEnd = pPattern->GetEnd ();

	for (ItemList::STLItemList_cit it = pPattern->GetBegin (); it != itEnd; ++it)
	{
		if (! mItemArray [static_cast<const Item *>(*it)->GetItemID ()])
		{
			bRet = false;
			break;
		}
	}

	return bRet;
}

const bool Transaction::IsCoveredBy (const Item *pItem) const
{
	return mItemArray [pItem->GetItemID ()];
}

const bool Transaction::IsCoveredByItem (const uint32 &rItemID) const
{
	return mItemArray [rItemID];
}

Item* Transaction::GetRandomItem () const
{
	uint32 itemIndex = Rand::GetRandomNumber (0, GetSize ());

	return static_cast<Item *>(GetAt (itemIndex));
}

PatternList* Transaction::GetPatternList (
		const float32 &support, const uint32 &projection_size,
		const uint32 &min_rule_len, const uint32 &max_rule_len,
		const PatternSet &rPatternSet) const
{
	PatternList *pPatternList = NULL;

	switch (rPatternSet)
	{
		case PATTERN_FREQUENT:
			{
				pPatternList = GetFrequentPatternList (support, projection_size, max_rule_len);

				LOGMSG (MEDIUM_LEVEL, "Transaction::GetPatternList () - remove short patterns\n");

				PatternList::STLPatternList_it it		= pPatternList->GetBegin ()	;
				PatternList::STLPatternList_it itPatternEnd	= pPatternList->GetEnd ()	;

				while (it != itPatternEnd)
				{
					if (static_cast<const Pattern*>(*it)->GetSize () < min_rule_len)
					{
						LOGMSG (MEDIUM_LEVEL, "Transaction::GetPatternList () - delete pattern [%s], frequence [%u]\n", static_cast<const Pattern*>(*it)->GetPrintableString ().c_str (), static_cast<const Pattern*>(*it)->GetFrequence ());
						delete *it;
						it = pPatternList->Erase (it);
						itPatternEnd = pPatternList->GetEnd ();
					}
					else
						break;
				}
			}

			break;
		case PATTERN_MAXIMAL:
			{
				PatternList *pFrequentPatternList = GetFrequentPatternList (support, projection_size, Item::GetMaxItemID () + 1);
				pPatternList = GetMaximalFrequentPatternList (pFrequentPatternList);

				PatternList::STLPatternList_cit itEnd = pFrequentPatternList->GetEnd ();

				for (PatternList::STLPatternList_cit it = pFrequentPatternList->GetBegin (); it != itEnd; ++it)
				{
					Pattern *pPattern = static_cast<Pattern *>(*it);

					if (! pPatternList->FindByPtr (pPattern))
						delete pPattern;
				}

				pFrequentPatternList->RemoveAll ();
				delete pFrequentPatternList;
			}

			break;
		case PATTERN_RAND_MAXIMAL:
			pPatternList = GetRandomMaximalFrequentPatternList (support, projection_size);
			break;
		default:
			LOGMSG (NO_DEBUG, "Transaction::GetPatternList () - unknown pattern set type\n");
			throw DataBaseException ("Unknown pattern set type");
			break;
	}

	Pattern::ResetSeqPatternID ();
	PatternList::STLPatternList_cit itEnd = pPatternList->GetEnd ();

	for (PatternList::STLPatternList_cit it = pPatternList->GetBegin (); it != itEnd; ++it)
		static_cast<Pattern *>(*it)->SetPatternID ();

	return pPatternList;
}

PatternList* Transaction::GetFrequentPatternList (
		const float32 &support, const uint32 &projection_size,
		const uint32 &rMaxPatternSize) const
{
	uint32 size = GetSize ();

	LOGMSG (LOW_LEVEL, "Transaction::GetFrequentPatternList () - items [%u], support [%f], projection_size [%u], rMaxPatternSize [%u]\n", size, support, projection_size, rMaxPatternSize);

	PatternList*	pFrequentPatternList	= new PatternList ()	;
	ItemList*	pFrequentItemList	= new ItemList ()	;

	LOGMSG (MEDIUM_LEVEL, "Transaction::GetFrequentPatternList () - add patterns size [1]\n");

	STLItemList_cit itEnd = GetEnd ();

	for (STLItemList_cit it = GetBegin (); it != itEnd; ++it)
	{
		Item *pItem = static_cast<Item *>(*it);

		if ((float32) pItem->GetProjectionFrequence () / projection_size >= support)
		{
			Pattern *pPattern = new Pattern (pItem);

			pPattern->SetSupport ((float32) pPattern->GetFrequence () / projection_size);
			pFrequentPatternList->PushBack (pPattern);

			pFrequentItemList->PushBack (pItem);
		}
	}

	uint32 numFrequentItems = pFrequentItemList->GetSize ();

	if (numFrequentItems > 0)
	{
		uint32 frequentItemBackID = static_cast<Item *>(pFrequentItemList->GetBack ())->GetItemID ();

		uint32 iTryPattern = 0;

		while (iTryPattern < pFrequentPatternList->GetSize ())
		{
			uint32 pattern_size = static_cast<const Pattern *>(pFrequentPatternList->GetAt (iTryPattern))->GetSize ();

			if (pattern_size < rMaxPatternSize)
			{
				LOGMSG (MEDIUM_LEVEL, "Transaction::GetFrequentPatternList () - add patterns size [%u]\n", pattern_size+1);

				uint32 iNumTryPatterns = pFrequentPatternList->GetSize ();

				do
				{
					Pattern *pPattern = static_cast<Pattern *>(pFrequentPatternList->GetAt (iTryPattern));
					uint32 pattern_size = pPattern->GetSize ();

					uint32 patternItemBackID = static_cast<Item *>(pPattern->GetBack ())->GetItemID ();

					if (patternItemBackID < frequentItemBackID)
					{
						for (uint32 i = pattern_size; i < numFrequentItems; i++)
						{
							Item *pItem = static_cast<Item *>(pFrequentItemList->GetAt (i));

							if (patternItemBackID < pItem->GetItemID ())
							{
								Pattern *pNewPattern = new Pattern (pPattern, pItem);

								if ((float32) pNewPattern->GetFrequence () / projection_size >= support)
								{
									pNewPattern->SetSupport ((float32) pNewPattern->GetFrequence () / projection_size);
									pFrequentPatternList->PushBack (pNewPattern);
									pPattern->AddChildPattern (pNewPattern);
								}
								else
									delete pNewPattern;
							}
						}
					}

					iTryPattern++;
				} while (iTryPattern < iNumTryPatterns);
			}
			else
				break;
		}
	}

	pFrequentItemList->RemoveAll ();
	delete pFrequentItemList;

	uint32 frequent_size = pFrequentPatternList->GetSize ();

	LOGMSG (LOW_LEVEL, "Transaction::GetFrequentPatternList () - patterns [%u]\n", frequent_size);

	return pFrequentPatternList;
}

PatternList* Transaction::GetMaximalFrequentPatternList (const PatternList *pFrequentPatternList) const
{
	uint32 size = GetSize ();

	LOGMSG (LOW_LEVEL, "Transaction::GetMaximalFrequentPatternList () - items [%u]\n", size);

	PatternList *pMaximalFrequentPatternList = NULL;

	if (pFrequentPatternList->GetSize ())
	{
		PatternList::STLPatternList_cit it	= pFrequentPatternList->GetBegin ();
		PatternList::STLPatternList_cit itEnd	= pFrequentPatternList->GetEnd ();

		Pattern *pPattern = static_cast<Pattern *>(*(it++));

		pMaximalFrequentPatternList = pPattern->GetMaximalPatternList ();

		while (it != itEnd)
		{
			pPattern = static_cast<Pattern *>(*(it++));

			if (pPattern->GetSize () > 1)
				break;

			PatternList *pCandidatePatternList = pPattern->GetMaximalPatternList ();

			PatternList::STLPatternList_cit itCandEnd = pCandidatePatternList->GetEnd ();

			for (PatternList::STLPatternList_cit itCand = pCandidatePatternList->GetBegin (); itCand != itCandEnd; ++itCand)
			{
				/*
				 * Insert candidate into maximal pattern
				 * list if there is no super-pattern of
				 * candidate in the list yet.
				 */

				Pattern *pCandidatePattern = static_cast<Pattern *>(*itCand);

				if (! pMaximalFrequentPatternList->FindSuperPatternOf (pCandidatePattern))
					pMaximalFrequentPatternList->PushBack (pCandidatePattern);
			}

			pCandidatePatternList->RemoveAll ();
			delete pCandidatePatternList;
		}
	}
	else
		pMaximalFrequentPatternList = new PatternList ();

	uint32 maximal_size = pMaximalFrequentPatternList->GetSize ();

	LOGMSG (LOW_LEVEL, "Transaction::GetMaximalFrequentPatternList () - patterns [%u]\n", maximal_size);

	return pMaximalFrequentPatternList;
}


Pattern* Transaction::GetRandomMaximalFrequentPattern (
		const float32 &support, const uint32 &projection_size) const
{
	const uint32 size = GetSize ();

	const uint32 num_items = Item::GetNumItems ();
	bool *triedItemArray = new bool [num_items];
	for (uint32 i = 0; i < num_items; i++)
		triedItemArray [i] = false;

	Pattern *pPattern = NULL;

	for (uint32 i = 0; i < size; i++)
	{
		Item *pItem = GetRandomItem ();

		if ((float32) pItem->GetProjectionFrequence () / projection_size >= support)
		{
			pPattern = new Pattern (pItem);
			triedItemArray [pItem->GetItemID ()] = true;
			break;
		}
	}

	if (pPattern)
	{
		uint32 tries = 0;

		while (tries < size)
		{
			Item *pItem = NULL;

			for (uint32 i = 0; i < size; i++)
			{
				pItem = GetRandomItem ();

				if (triedItemArray [pItem->GetItemID ()])
					pItem = NULL;
				else
					break;
			}

			if (pItem)
			{
				triedItemArray [pItem->GetItemID ()] = true;

				Pattern *pNewPattern = new Pattern (pPattern, pItem);

				if ((float32) pNewPattern->GetFrequence () / projection_size >= support)
				{
					pNewPattern->SetSupport ((float32) pNewPattern->GetFrequence () / projection_size);
					pPattern->RemoveAll ();
					delete pPattern;
					pPattern = pNewPattern;
//					tries = 0;
				}
				else
				{
					pNewPattern->RemoveAll ();
					delete pNewPattern;
					tries++;
				}
			}
			else
				tries++;
		}
	}

	delete[] triedItemArray;

	return pPattern;
}

PatternList* Transaction::GetRandomMaximalFrequentPatternList (
		const float32 &support, const uint32 &projection_size) const
{
	uint32 size = GetSize ();

	LOGMSG (LOW_LEVEL, "Transaction::GetRandomMaximalFrequentPatternList () - support [%f], projection_size [%u], size [%u]\n", support, projection_size, size);

	Rand::SetSeed ();

	PatternList *pPatternList = new PatternList ();

	uint32 tries = 0;

	while (tries < size)
	{
//		LOGMSG (LOW_LEVEL, "Transaction::GetRandomMaximalFrequentPatternList () - tries [%u]\n", tries);

		Pattern *pPattern = GetRandomMaximalFrequentPattern (support, projection_size);

		if (pPattern)
		{
			pPattern->Sort ();

			if (pPatternList->FindSuperPatternOf (pPattern))
			{
				delete pPattern;
				tries++;
			}
			else
			{
				pPatternList->RemoveSubPatternsOf (pPattern);
				pPatternList->PushBack (pPattern);
//				tries = 0;
			}
		}
		else
			tries++;
	}

	size = pPatternList->GetSize ();

	LOGMSG (LOW_LEVEL, "Transaction::GetRandomMaximalFrequentPatternList () - patterns [%u]\n", size);

	return pPatternList;
}

void Transaction::MakeItemCoverageArray ()
{
	const uint32 num_items = Item::GetNumItems ();
	mItemArray = new bool [num_items];
	for (uint32 i = 0; i < num_items; i++)
		mItemArray [i] = false;

	STLItemList_cit itEnd = GetEnd ();

	for (STLItemList_cit it = GetBegin (); it != itEnd; ++it)
		mItemArray [static_cast<const Item *>(*it)->GetItemID ()] = true;
}

void Transaction::AddTransactionToItemsProjectionTransactionLists ()
{
	STLItemList_cit itEnd = GetEnd ();

	for (STLItemList_cit it = GetBegin (); it != itEnd; ++it)
		static_cast<Item *>(*it)->AddProjectionTransaction (this);
}

void Transaction::Print () const
{
	LOGMSG (LOW_LEVEL, "Transaction::Print () - mTransactionID [%u], class [%s], items [%s]\n",  mTransactionID, mpClass->GetValue ().c_str (), GetPrintableString ().c_str ());
}
