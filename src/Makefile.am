LDADD_BASE	= $(top_builddir)/src/base/libbase.la

SRCS		= Class.cpp ClassList.cpp DataBase.cpp IOManager.cpp		\
	Item.cpp ItemHash.cpp ItemList.cpp ItemSet.cpp Pattern.cpp		\
	PatternList.cpp Rule.cpp RuleList.cpp RuleUIntHash.cpp			\
	Tokenizer.cpp Transaction.cpp TransactionList.cpp AppOptions.cpp	\
	main.cpp

SUBDIRS = base

bin_PROGRAMS		= classifier
classifier_SOURCES	= $(SRCS)
classifier_LDADD	= $(LDADD_BASE)
