%%
%% This is file `ufmgthesis.cls',
%% generated with the docstrip utility.
%%
%% The original source files were:
%%
%% ufmgthesis.dtx  (with options: `class')
%% 
%% This is a generated file.
%% 
%% Copyright (C) 2004,2005 by Vilar Camara Neto and Eduardo Nakamura.
%% 
%% This file may be distributed and/or modified under the
%% conditions of the LaTeX Project Public License, either
%% version 1.2 of this license or (at your option) any later
%% version.  The latest version of this license is in:
%% 
%%     http://www.latex-project.org/lppl.txt
%% 
%% and version 1.2 or later is part of all distributions of
%% LaTeX version 1999/12/01 or later.
%% 
%% Currently this work has the LPPL maintenance status "maintained".
%% 
%% The Current Maintainer of this work is Vilar Camara Neto.
%% 
%% This work consists of the files ufmgthesis.dtx and
%% ufmgthesis.ins and the derived file ufmgthesis.cls.
%% 
\NeedsTeXFormat{LaTeX2e}[1999/12/01]
\ProvidesClass{ufmgthesis}
  [2008/02/26 v0.31 Classe para dissertações, teses e propostas de tese no
  formato PPGCC/ICEx/UFMG]


\RequirePackage{keyval}
\RequirePackage{setspace}

%% Opções gerais:

\newif\if@phd                 % Tese de doutorado?
\newif\if@msc                 % Dissertação de mestrado?
\newif\if@proposal            % Opção de proposta/projeto?
\newif\if@showcover           % Compor a capa?
\newif\if@showtitle           % Compor a folha de título?
\newif\if@showapproval        % Compor a folha de aprovação?
\newif\if@showabstract        % Compor as páginas de resumo/abstract?
\newif\if@showdedication      % Compor a dedicatória?
\newif\if@showack             % Compor os agradecimentos?
\newif\if@showtoc             % Compor o Sumário?
\newif\if@showlof             % Compor a Lista de Figuras?
\newif\if@showlot             % Compor a Lista de Tabelas?
\newif\if@extraporttitlepage  % Exibir também uma folha de título em português?
\newif\if@putmainmatter       % \mainmatter depois das páginas introdutórias?

\def\ufmg@showall{%
  \@showcovertrue
  \@showtitletrue
  \@showapprovaltrue
  \@showabstracttrue
  \@showdedicationtrue
  \@showacktrue
  \@showtoctrue
  \@showloftrue
  \@showlottrue
}

\def\ufmg@hideall{%
  \@showcoverfalse
  \@showtitlefalse
  \@showapprovalfalse
  \@showabstractfalse
  \@showdedicationfalse
  \@showackfalse
  \@showtocfalse
  \@showloffalse
  \@showlotfalse
}

\@proposalfalse
\ufmg@showall
\@putmainmattertrue

%% Opções das referências bibliográficas:

\newif\if@bibnorepauthor  % Substituir autorias repetidas por linha comprida?
\newif\if@nobreakitems    % Impedir quebra de página no meio de um item?

\@bibnorepauthorfalse
\@nobreakitemsfalse

\def\BreakableUppercase#1{%
  \bgroup
  \let\ufmg@prevdbs=\\%
  \def\\{\protect\ufmg@prevdbs}%
  \MakeUppercase{#1}%
  \egroup
}

\newcommand{\ufmg@defspacing}{\onehalfspace}

\def\ufmg@redefchaptitlefont{}

\newcommand*{\university}[1]{\gdef\@university{#1}}  % Nome da universidade
\newcommand*{\course}[1]{\gdef\@course{#1}}          % Nome do curso
\newcommand*{\address}[1]{\gdef\@address{#1}}        % Endereço (cidade/estado)

\newcommand*{\degree}[1]{\gdef\@degree{#1}}          % Titulação
\newcommand*{\logo}[1]{\gdef\@logo{#1}}              % Logotipo
\renewcommand*{\title}[1]                            % Título
  {\gdef\@title{#1}}

\newcommand*{\portuguesetitle}[1]                    % Título em português
  {\gdef\@portuguesetitle{#1}}
\newcommand*{\portugueseuniversity}[1]               % Nome da univers. em pt.
  {\gdef\@portugueseuniversity{#1}}
\newcommand*{\portuguesecourse}[1]                   % Nome do curso em port.
  {\gdef\@portuguesecourse{#1}}

\DeclareOption{msc}{\@phdfalse\@msctrue}
\DeclareOption{phd}{\@phdtrue\@mscfalse}
\DeclareOption{proposal}{\@proposaltrue}
\DeclareOption{project}{\@proposaltrue}

\DeclareOption{single}{\renewcommand{\ufmg@defspacing}{\singlespace}}
\DeclareOption{onehalf}{\renewcommand{\ufmg@defspacing}{\onehalfspace}}
\DeclareOption{double}{\renewcommand{\ufmg@defspacing}{\doublespace}}

\DeclareOption{hideall}{\ufmg@hideall}
\DeclareOption{hidecover}{\@showcoverfalse}
\DeclareOption{hidetitle}{\@showtitlefalse}
\DeclareOption{hideapproval}{\@showapprovalfalse}
\DeclareOption{hideabstract}{\@showabstractfalse}
\DeclareOption{hidededication}{\@showdedicationfalse}
\DeclareOption{hideack}{\@showackfalse}
\DeclareOption{hidetoc}{\@showtocfalse}
\DeclareOption{hidelof}{\@showloffalse}
\DeclareOption{hidelot}{\@showlotfalse}

\DeclareOption{showall}{\ufmg@showall}
\DeclareOption{showcover}{\@showcovertrue}
\DeclareOption{showtitle}{\@showtitletrue}
\DeclareOption{showapproval}{\@showapprovaltrue}
\DeclareOption{showabstract}{\@showabstracttrue}
\DeclareOption{showdedication}{\@showdedicationtrue}
\DeclareOption{showack}{\@showacktrue}
\DeclareOption{showtoc}{\@showtoctrue}
\DeclareOption{showlof}{\@showloftrue}
\DeclareOption{showlot}{\@showlottrue}

\DeclareOption{nomainmatter}{\@putmainmatterfalse}

\DeclareOption{centertitles}{%
   \def\ufmg@redefchaptitlefont{%
    \expandafter\renewcommand\expandafter{\expandafter\chaptitlefont
    \expandafter}\expandafter{\chaptitlefont\centering}%
  }%
}

\DeclareOption*{\PassOptionsToClass{\CurrentOption}{memoir}}

\ExecuteOptions{phd,onehalf,showall}
\ProcessOptions

\def\ufmg@setdoctype{%
  \if@phd
    % Esta é uma tese de doutorado
    \degree{\ufmg@phd}
    \if@proposal
      \gdef\@documenttype{\ufmg@phddocproj}%
      \ufmg@set@phddocproj
    \else
      \gdef\@documenttype{\ufmg@phddoc}%
      \ufmg@set@phddoc
    \fi
  \else
    % Esta é uma dissertação de mestrado
    \degree{\ufmg@msc}
    \if@proposal
      \gdef\@documenttype{\ufmg@mscdocprop}%
      \ufmg@set@mscdocprop
    \else
      \gdef\@documenttype{\ufmg@mscdoc}%
      \ufmg@set@mscdoc
    \fi
  \fi
}

\LoadClass{memoir}

\setlrmarginsandblock{3cm}{2.5cm}{*}
\setulmarginsandblock{3cm}{3cm}{*}

\checkandfixthelayout

\maxsecnumdepth{subsubsection}
\setcounter{tocdepth}{2}
\setcounter{secnumdepth}{4}

\def\ufmg@langgroup{\@ifundefined{languagename}{brazil}{\languagename}}

\def\ufmg@deflanguage#1{%
  \@ifundefined{ufmg@lang@#1}{%
    \ClassWarning{ufmgthesis}{Linguagem nao definida: #1^^J
    Adotando a linguagem padrao `brazil'.^^J}%
    \def\ufmg@langgroup{brazil}}
    {\def\ufmg@langgroup{#1}}%
}
\def\ufmg@selectlanguage{%
  \csname ufmg@lang@\ufmg@langgroup\endcsname\ufmg@setdoctype
}

\newif\if@ufmgdateerror

\newcommand{\ufmg@formatdate}[2][]{%
  \begingroup
    \def\ufmg@templang{#1}%
    \ifx\ufmg@templang\empty\let\ufmg@templang\ufmg@langgroup\fi
    \expandafter\ufmg@parsedate#2---\relax
  \endgroup
}
\def\ufmg@parsedate#1-#2-#3-#4\relax{%
  \@ufmgdateerrortrue
  \def\ufmg@year{\number#1}\def\ufmg@month{\number#2}%
  \def\ufmg@day{#3}\ifx\ufmg@day\empty\else\def\ufmg@day{\number#3}\fi
  \ifx\ufmg@month\empty\else\ifnum#1>99\relax
    \@ufmgdateerrorfalse
    \csname ufmg@expanddate@\ufmg@templang\endcsname
  \fi\fi
  \if@ufmgdateerror
    \ClassError{ufmgthesis}{Formato invalido de data.^^J%
    Ao usar a opcao `date=' para o comando \string\ufmgthesis\space ou o
    comando \string\date\string{...\string},^^J%
    sempre forneca a data no formato aaaa-mm-dd ou aaaa-mm^^J%
    (note a ordem ano-mes-dia e o hifen como separador)}%
    \endinput
  \fi
}

\def\ufmg@expandmonth@brazil#1{%
  \ifcase\ufmg@month\or
  janeiro\or fevereiro\or mar\c{c}o\or abril\or maio\or junho\or
  julho\or agosto\or setembro\or outubro\or novembro\or dezembro\fi
}
\def\ufmg@expanddate@brazil{%
  \edef\text@month{\ufmg@expandmonth@brazil{\ufmg@month}}%
  \ifx\ufmg@day\empty\expandafter\MakeUppercase\text@month
    \else \two@digits\ufmg@day\ de\ \text@month\fi
  \ de\ \ufmg@year
}
\def\ufmg@lang@brazil{%
  \@extraporttitlepagefalse

  \def\@deflang@title{%
    \@ifundefined{@portuguesetitle}{\@title}{\@portuguesetitle}}
  \def\@deflang@university{%
    \@ifundefined{@portugueseuniversity}{\@university}%
    {\@portugueseuniversity}}
  \def\@deflang@course{%
    \@ifundefined{@portuguesecourse}{\@course}{\@portuguesecourse}}

  \def\ufmg@msc{Mestre}
  \def\ufmg@phd{Doutor}
  \def\ufmg@mscdoc{Disserta\c{c}\~{a}o}
  \def\ufmg@mscdocprop{Proposta de disserta\c{c}\~{a}o}
  \def\ufmg@phddoc{Tese}
  \def\ufmg@phddocproj{Projeto de tese}
  \def\ufmg@approvaltitle{Folha de Aprova\c{c}\~{a}o}
  \def\ufmg@advisor{Orientador}
  \def\ufmg@coadvisor{Co-orientador}
  \def\ufmg@docdescription{%
    \@documenttype{} apresentad\ufmg@doctype@gender{} ao Programa de
    P\'{o}s-Gradua\c{c}\~{a}o em \@deflang@course{} da
    \@deflang@university{} como requisito parcial para a obten\c{c}\~{a}o
    do grau de \@degree{} em \@deflang@course.}
  \def\ufmg@approvaltext{%
    \@documenttype{} defendid\ufmg@doctype@gender{} e
    aprovad\ufmg@doctype@gender{} pela banca examinadora constitu\'{i}da
    por:}
  \def\ufmg@acknowledgments{Agradecimentos}

  \def\ufmg@set@phddocproj{\def\ufmg@doctype@gender{o}}
  \def\ufmg@set@phddoc{\def\ufmg@doctype@gender{a}}
  \def\ufmg@set@mscdocprop{\def\ufmg@doctype@gender{a}}
  \def\ufmg@set@mscdoc{\def\ufmg@doctype@gender{a}}

  \def\ufmgbibauthorsep{;}
  \def\ufmgbibauthorlastsep{}
  \def\ufmgbibauthorand{e}
  \def\ufmgciteauthorand{e}
  \def\ufmgbibpagestring{p.\@}
  \def\ufmgbibpagesstring{pp.\@}
  \def\ufmgbibeditorstring{editor}
  \def\ufmgbibeditorsstring{editores}
  \def\ufmgbibeditionstring{edição}

  \def\appendixpagename{Apêndices}
  \def\appendixtocname{Apêndices}
}

\def\ufmg@expandmonth@english#1{%
  \ifcase\ufmg@month\or
  january\or february\or march\or april\or may\or june\or
  july\or august\or september\or october\or november\or december\fi
}
\def\ufmg@expanddate@english{%
  \edef\text@month{\ufmg@expandmonth@english{\ufmg@month}}%
  \expandafter\MakeUppercase\text@month
  \ifx\ufmg@day\empty\else\ \ufmg@day,\fi
  \ \ufmg@year
}
\def\ufmg@lang@english{%
  \@extraporttitlepagetrue

  \def\@deflang@title{\@title}
  \def\@deflang@university{\@university}
  \def\@deflang@course{\@course}

  \def\ufmg@msc{Master}
  \def\ufmg@phd{Doctor}
  \def\ufmg@mscdoc{Dissertation}
  \def\ufmg@mscdocprop{Dissertation proposal}
  \def\ufmg@phddoc{Thesis}
  \def\ufmg@phddocproj{Thesis project}
  \def\ufmg@approvaltitle{Approval Sheet}
  \def\ufmg@advisor{Advisor}
  \def\ufmg@coadvisor{Co-advisor}
  \def\ufmg@docdescription{%
    \@documenttype{} presented to the Graduate Program in \@course{} of the
    \@university{} in partial fulfillment of the requirements for the
    degree of \@degree{} in \@course.}
  \def\ufmg@approvaltext{%
    \@documenttype{} presented to and approved by the committee composed by:}
  \def\ufmg@acknowledgments{Acknowledgments}

  \let\ufmg@set@phddocproj\relax
  \let\ufmg@set@phddoc\relax
  \let\ufmg@set@mscdocprop\relax
  \let\ufmg@set@mscdoc\relax

  \def\ufmgbibauthorsep{,}
  \def\ufmgbibauthorlastsep{,}
  \def\ufmgbibauthorand{and}
  \def\ufmgciteauthorand{and}
  \def\ufmgbibpagestring{page}
  \def\ufmgbibpagesstring{pages}
  \def\ufmgbibeditorstring{editor}
  \def\ufmgbibeditorsstring{editors}
  \def\ufmgbibeditionstring{edition}
}
\newcommand{\makecoverpage}{%
  \if@showcover\begingroup
    \ufmg@deflanguage{brazil}\ufmg@selectlanguage
    \ufmg@maketitlepage{\z@}
  \endgroup\fi
}
\newcommand{\maketitlepage}{%
  \if@showtitle
    \if@extraporttitlepage\begingroup
      \ufmg@deflanguage{brazil}\ufmg@selectlanguage
      \ufmg@maketitlepage{\@ne}%
    \endgroup\fi
    \ufmg@maketitlepage{\@ne}%
  \fi
}

\def\ufmg@docdescriptionwidth{0.5\textwidth}

\def\ufmg@makedocdescription{%
  \begin{minipage}{\ufmg@docdescriptionwidth}
    \ufmg@docdescription
  \end{minipage}
}

\def\ufmg@titpag@fmtauthor#1{\large\BreakableUppercase{#1}}
\def\ufmg@titpag@fmtadvisor{\large\scshape}
\def\ufmg@titpag@fmttitle#1{\Large\bfseries\BreakableUppercase{#1}}
\def\ufmg@titpag@fmtaddress{\large}
\def\ufmg@titpag@fmtdate{\large}

\def\ufmg@maketitlepage#1{%
  \begin{titlingpage}
  \begin{singlespace}
  {\centering
    \vbox to48ex{%
      {\ufmg@titpag@fmtauthor{\@author}\par}%
      \ifx#1\z@
      \else
        \medskip
        {\ufmg@titpag@fmtadvisor{\ufmg@advisor: \ufmg@advisorname}\par}%
      \fi
      \vfil
    }%

    \begin{doublespace}
    {\ufmg@titpag@fmttitle{\@deflang@title}\par}

    \vspace{10ex}
    \end{doublespace}

    \ifx#1\@ne
      \hspace*{\stretch{1}}\ufmg@makedocdescription\par
    \fi

    \vspace{\stretch{1}}

    {\ufmg@titpag@fmtaddress{\@address}\par}
    \vspace{1ex}

    {\ufmg@titpag@fmtdate{\ufmg@formatdate{\@date}\par}}
  }
  \end{singlespace}
  \end{titlingpage}
}

\newcounter{ufmg@c@banca}

\newcommand{\addtocomitee}[4][\relax]{%
  \stepcounter{ufmg@c@banca}
  \expandafter\def\csname ufmg@banca@\theufmg@c@banca name\endcsname
    {#2}
  \expandafter\def\csname ufmg@banca@\theufmg@c@banca title\endcsname
    {#3}
  \def\ufmg@temp{#4}
  \if\ufmg@temp.
    \expandafter\def\csname ufmg@banca@\theufmg@c@banca instit\endcsname
      {\@university}
  \else
    \expandafter\def\csname ufmg@banca@\theufmg@c@banca instit\endcsname
      {#4}
  \fi
  \ifx#1\relax\else
    \expandafter\def\csname ufmg@banca@\theufmg@c@banca rel\endcsname
      {#1}
  \fi
}

\newcommand{\advisor}[3]{%
  \addtocomitee[\ufmg@advisor]{#1}{#2}{#3}%
  \def\ufmg@advisorname{#1}%
}
\newcommand{\coadvisor}[3]{\addtocomitee[\ufmg@coadvisor]{#1}{#2}{#3}}

\def\ufmg@fmtbancatitle{}
\def\ufmg@fmtbancaname{\scshape}
\def\ufmg@fmtbancainstit{}
\def\ufmg@fmtbancarel{}

\newcommand{\ufmg@makecomiteelist}{%
  \setcounter{ufmg@c@banca}{1}
  \loop\expandafter\ifx\csname ufmg@banca@\theufmg@c@banca name\endcsname\relax
    \else
    {\centering
    \vspace{\stretch{1}}
    {\ufmg@fmtbancatitle{%
      \csname ufmg@banca@\theufmg@c@banca title\endcsname}}~%
    {\ufmg@fmtbancaname{%
      \csname ufmg@banca@\theufmg@c@banca name\endcsname}}%
    \@ifundefined{ufmg@banca@\theufmg@c@banca rel}{}{%
      \ -- {\ufmg@fmtbancarel{%
        \csname ufmg@banca@\theufmg@c@banca rel\endcsname}}%
    }%
    \\%
    {\ufmg@fmtbancainstit{%
      \csname ufmg@banca@\theufmg@c@banca instit\endcsname}}\\%
    }
    \stepcounter{ufmg@c@banca}
  \repeat
  \vspace{\stretch{1.5}}
}

\def\ufmg@apprpag@fmtuniv#1{\normalsize\MakeUppercase{#1}}
\def\ufmg@apprpag@fmtcaption#1{\large\MakeUppercase{#1}}
\def\ufmg@apprpag@fmttitle{\Large}
\def\ufmg@apprpag@fmtauthor#1{\large\BreakableUppercase{#1}}
\def\ufmg@apprpag@fmttext{}
\def\ufmg@apprpag@fmtaddrdate{}

\newcommand{\makeapproval}{%
  \if@showapproval
    \begingroup
    \ufmg@deflanguage{brazil}\ufmg@selectlanguage
    \begin{titlingpage}
    \begin{singlespace}
    {\centering
      \ifx\@logo\@undefined
        {\ufmg@apprpag@fmtuniv{\@deflang@university}\par}
        \vspace{12ex}
      \else
        \raisebox{-0.5\height}{\includegraphics{\@logo}}\quad
        {\ufmg@apprpag@fmtuniv{\@deflang@university}\par}
        \vspace{6ex}
      \fi

      {\ufmg@apprpag@fmtcaption{\ufmg@approvaltitle}\par}
      \vspace{5ex}

      {\ufmg@apprpag@fmttitle{\@deflang@title}\par}
      \vspace{5ex}

      {\ufmg@apprpag@fmtauthor{\@author}\par}
      \vspace{5ex}
    }

    {\ufmg@apprpag@fmttext{\ufmg@approvaltext}\par}
    \vspace{4ex}

    \ufmg@makecomiteelist

    {\centering
      {\ufmg@apprpag@fmtaddrdate{\@address, \ufmg@formatdate{\@date}}\par}%
    }
    \end{singlespace}
    \end{titlingpage}
    \endgroup
  \fi
}

\newcommand{\includeabstract}[2]{%
  \if@showabstract
    \ufmg@redefchaptitlefont
    \chapter*{#1}
    \input{#2}
    \cleardoublepage
  \fi
}

\newcounter{ufmg@c@abstract}

\newcommand{\addabstract}[2]{%
  \stepcounter{ufmg@c@abstract}
  \expandafter\def\csname ufmg@abstract@\theufmg@c@abstract title\endcsname
    {#1}
  \expandafter\def\csname ufmg@abstract@\theufmg@c@abstract filename\endcsname
    {#2}
}

\newcommand{\ufmg@includeabstractlist}{%
  \if@showabstract
    \setcounter{ufmg@c@abstract}{1}
    \loop\expandafter\ifx
      \csname ufmg@abstract@\theufmg@c@abstract title\endcsname
      \relax
      \else
      \includeabstract
        {\csname ufmg@abstract@\theufmg@c@abstract title\endcsname}
        {\csname ufmg@abstract@\theufmg@c@abstract filename\endcsname}
      \stepcounter{ufmg@c@abstract}
    \repeat
  \fi
}

\newcounter{ufmg@c@dedication}
\newcounter{ufmg@c@ack}

\newcommand{\adddedication}[1]{%
  \stepcounter{ufmg@c@dedication}
  \expandafter\def\csname ufmg@dedication@\theufmg@c@dedication
    filename\endcsname{#1}
}

\newcommand{\addack}[2][]{%
  \stepcounter{ufmg@c@ack}
  \def\ufmg@tempack{#1}
  \ifx\ufmg@tempack\empty
    \expandafter\addack@\expandafter{\ufmg@acknowledgments}
  \else
    \addack@{#1}
  \fi
  \expandafter\def\csname ufmg@ack@\theufmg@c@ack filename\endcsname{#2}
}
\def\addack@#1{
  \expandafter\def\csname ufmg@ack@\theufmg@c@ack title\endcsname{#1}
}

\newcommand{\includededication}[1]{%
  \if@showdedication
    \vspace*{\stretch{1}}
    \textit{\input{#1}}
    \vspace*{\stretch{1}}
    \cleardoublepage
  \fi
}

\newcommand{\includeack}[2][]{%
  \if@showack
    \edef\ufmg@tempack{#1}%
    \ifx\ufmg@tempack\empty\let\ufmg@tempack\ufmg@acknowledgments\fi
    \ufmg@redefchaptitlefont
    \chapter*{\ufmg@tempack}
    \input{#2}
    \cleardoublepage
  \fi
}

\newcommand{\ufmg@includededicationlist}{%
  \if@showdedication
    \setcounter{ufmg@c@dedication}{1}
    \loop
    \expandafter\ifx
      \csname ufmg@dedication@\theufmg@c@dedication filename\endcsname
      \relax
    \else
      \includededication
        {\csname ufmg@dedication@\theufmg@c@dedication filename\endcsname}
      \stepcounter{ufmg@c@dedication}
    \repeat
  \fi
}
\newcommand{\ufmg@includeacklist}{%
  \if@showack
    \setcounter{ufmg@c@ack}{1}
    \loop
    \expandafter\ifx\csname ufmg@ack@\theufmg@c@ack filename\endcsname\relax
    \else
      \includeack
        [\csname ufmg@ack@\theufmg@c@ack title\endcsname]
        {\csname ufmg@ack@\theufmg@c@ack filename\endcsname}
      \stepcounter{ufmg@c@ack}
    \repeat
  \fi
}

\if@twoside
  \makeheadrule{headings}{\textwidth}{\normalrulethickness}
  \makeoddhead{headings}{\textsc{\rightmark}}{}{\thepage}
  \makeevenhead{headings}{\thepage}{}{\textsc{\leftmark}}

  \copypagestyle{contents}{plain}

  \copypagestyle{listoffigures}{plain}

  \copypagestyle{listoftables}{plain}

  \copypagestyle{bibliography}{headings}
  \makeoddhead{bibliography}{\textsc{\bibname}}{}{\thepage}
  \makeevenhead{bibliography}{\thepage}{}{\textsc{\bibname}}
\else
  \makeheadrule{headings}{\textwidth}{\normalrulethickness}
  \makeoddhead{headings}{\textsc{\leftmark}}{}{\thepage}

  \copypagestyle{contents}{plain}

  \copypagestyle{listoffigures}{plain}

  \copypagestyle{listoftables}{plain}

  \copypagestyle{bibliography}{headings}
  \makeoddhead{bibliography}{\textsc{\bibname}}{}{\thepage}
\fi

\def\ufmg@chaptermark#1{%
  \markboth{%
    \ifnum\c@secnumdepth>\m@ne
      \if@mainmatter
        \if@twoside\@chapapp\ \fi
        \thechapter.
      \fi
    \fi
    #1}{}}%

\def\ufmg@sectionmark#1{%
  \markright{%
    \ifnum\c@secnumdepth>\z@\thesection. \ \fi
    #1}}%

\def\ufmgrestoremarks{%
  \let\chaptermark=\ufmg@chaptermark
  \let\sectionmark=\ufmg@sectionmark
}

\ufmgrestoremarks
\g@addto@macro\mainmatter{\pagestyle{headings}\ufmgrestoremarks}

\let\ufmg@orig@tableofcontents=\tableofcontents
\renewcommand{\tableofcontents}{{%
  \if@showtoc\begingroup
    \ufmg@redefchaptitlefont
    \let\ufmg@save@tableofcontents=\tableofcontents
    \let\tableofcontents=\ufmg@orig@tableofcontents
    \pagestyle{contents}
    \tableofcontents*
    \cleardoublepage
  \endgroup\fi
}}

\let\ufmg@orig@listoffigures=\listoffigures
\renewcommand{\listoffigures}{{%
  \if@showlof\begingroup
    \ufmg@redefchaptitlefont
    \let\ufmg@save@listoffigures=\listoffigures
    \let\listoffigures=\ufmg@orig@listoffigures
    \pagestyle{listoffigures}
    \listoffigures*
    \cleardoublepage
  \endgroup\fi
}}

\let\ufmg@orig@listoftables=\listoftables
\renewcommand{\listoftables}{{%
  \if@showlot\begingroup
    \ufmg@redefchaptitlefont
    \let\ufmg@save@listoftables=\listoftables
    \let\listoftables=\ufmg@orig@listoftables
    \pagestyle{listoftables}
    \listoftables*
    \cleardoublepage
  \endgroup\fi
}}


\def\ufmg@addabstract#1#2{\addabstract{#1}{#2}}
\def\ufmg@adddedication#1{\adddedication{#1}}
\def\ufmg@addack{\@ifnextchar[{\addack}{\ufmg@addack@}}
\def\ufmg@addack@#1\relax{\addack[]{#1}}

\define@key{ufmg}{title}{\title{#1}}
\define@key{ufmg}{author}{\author{#1}}
\define@key{ufmg}{university}{\university{#1}}
\define@key{ufmg}{course}{\course{#1}}
\define@key{ufmg}{portuguesetitle}{\portuguesetitle{#1}}
\define@key{ufmg}{portugueseuniversity}{\portugueseuniversity{#1}}
\define@key{ufmg}{portuguesecourse}{\portuguesecourse{#1}}
\define@key{ufmg}{address}{\address{#1}}
\define@key{ufmg}{date}{\date{#1}}
\define@key{ufmg}{logo}{\logo{#1}}
\define@key{ufmg}{advisor}{\advisor#1}
\define@key{ufmg}{coadvisor}{\coadvisor#1}
\define@key{ufmg}{member}{\addtocomitee#1}
\define@key{ufmg}{abstract}{\ufmg@addabstract#1}
\define@key{ufmg}{dedication}{\ufmg@adddedication{#1}}
\define@key{ufmg}{ack}{\ufmg@addack#1\relax}

\newcommand{\ufmgthesis}[1]{%
  \setkeys{ufmg}{#1}

  \frontmatter
  \pagestyle{plain}

  % Capa
  \makecoverpage
  % Folha(s) de título
  \maketitlepage
  % Folha de aprovação
  \makeapproval
  % Dedicatória
  \ufmg@includededicationlist
  % Agradecimentos
  \ufmg@includeacklist
  % Resumo, Abstract, etc.
  \ufmg@includeabstractlist

  \tableofcontents
  \listoffigures
  \listoftables

  \if@putmainmatter\mainmatter\fi
}

\def\ufmg@bibauthorformat{}
\def\ufmg@bibtitleformat{}
\def\ufmg@bibbtitleformat{\itshape}
\def\ufmg@bibbooktitleformat{\itshape}
\def\ufmg@bibjournalformat{\itshape}

\def\ufmg@norepstring{---------}
\def\ufmgbibinstring{In}

\newcommand{\ufmgbibauthordoformat}[1]{%
  \if@bibnorepauthor
    \def\ufmg@currentauthor{#1}%
    \ifx\ufmg@lastauthor\ufmg@currentauthor\ufmg@norepstring
      \else{\ufmg@bibauthorformat#1}\fi
    \def\ufmg@lastauthor{#1}%
  \else
    {\ufmg@bibauthorformat#1}%
  \fi
}
\newcommand{\ufmgbibtitledoformat}[1]{{\ufmg@bibtitleformat#1}}
\newcommand{\ufmgbibbtitledoformat}[1]{{\ufmg@bibbtitleformat#1}}
\newcommand{\ufmgbibbooktitledoformat}[1]{{\ufmg@bibbooktitleformat#1}}
\newcommand{\ufmgbibjournaldoformat}[1]{{\ufmg@bibjournalformat#1}}

\define@key{ufmgbib}{noauthorrepeat}[true]{\@bibnorepauthortrue}
\define@key{ufmgbib}{noauthorrepstring}{\def\ufmg@norepstring{#1}}
\define@key{ufmgbib}{nobreakitems}[true]{\@nobreakitemstrue}
\define@key{ufmgbib}{bibauthorand}{\def\ufmgbibauthorand{#1}}
\define@key{ufmgbib}{citeauthorand}{\def\ufmgciteauthorand{#1}}
\define@key{ufmgbib}{authorformat}{\def\ufmg@bibauthorformat{#1}}
\define@key{ufmgbib}{titleformat}{\def\ufmg@bibtitleformat{#1}}
\define@key{ufmgbib}{btitleformat}{\def\ufmg@bibbtitleformat{#1}}
\define@key{ufmgbib}{booktitleformat}{\def\ufmg@bibbooktitleformat{#1}}
\define@key{ufmgbib}{journalformat}{\def\ufmg@bibjournalformat{#1}}

\newcommand{\ufmgbibliography}[2][]{%
  \setkeys{ufmgbib}{#1}

  \cleardoublepage
  \pagestyle{bibliography}
  \bibliographystyle{ufmg}
  \begingroup
    \ufmg@redefchaptitlefont
    \if@nobreakitems\raggedbottom\interlinepenalty=10000\relax\fi
    \bibliography{#2}
  \endgroup

  \cleardoublepage
  \pagestyle{headings}
  \ufmgrestoremarks
}

\AtBeginDocument{%
  \ufmg@selectlanguage\ufmg@defspacing
  \@ifpackageloaded{graphicx}{}{\RequirePackage{graphicx}}%
  \@ifpackageloaded{hyperref}{\RequirePackage{memhfixc}}{}%
}
\endinput
%%
%% End of file `ufmgthesis.cls'.
