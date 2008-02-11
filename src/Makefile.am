LDADD_BASE	= $(top_builddir)/src/base/libbase.la

SRCS		= Class.cpp ClassList.cpp DataBase.cpp Transaction.cpp		\
	Item.cpp ItemHash.cpp ItemList.cpp ItemSet.cpp Pattern.cpp		\
	PatternList.cpp Rule.cpp RankingRule.cpp RankingRuleList.cpp		\
	RuleUIntHash.cpp AppOptions.cpp	TransactionList.cpp			\
	DataBaseException.cpp main.cpp

SUBDIRS = base

bin_PROGRAMS		= classifier
classifier_SOURCES	= $(SRCS)
classifier_LDADD	= $(LDADD_BASE)
classifier_CPPFLAGS	= -pg
classifier_LDFLAGS	= -pg
