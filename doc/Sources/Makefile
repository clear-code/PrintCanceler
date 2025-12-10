# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

DATE=$(shell date +%Y%m%d)
BASE=$(shell pwd)
OUTDIR=$(BASE)/../Outs
# TOOLDIR=$(BASE)/../Tools

# How to generate a PDF document:
#
#   $ sudo apt install texlive-full pandoc fonts-noto-cjk
#   $ make (Name of Document)-pdf
#
PANDOC_OPT_PDF= -N --toc-depth=2 --table-of-contents \
                -f markdown+escaped_line_breaks \
                --wrap=preserve \
                -s --listings \
                --template=custom-eisvogel.latex \
                --lua-filter=line-break.lua \
                -V documentclass=ltjsarticle \
                -V classoption=titlepage \
                -V header-right=" " \
                -V table-use-row-colors=true \
                --pdf-engine=xelatex

PANDOC_OPT_DOCX= --toc-depth=2 --table-of-contents \
                 -f markdown+east_asian_line_breaks \
                 -t docx \
                 --reference-doc="$(BASE)/assets/style-reference.docx"

all:  user-guide troubleshooting-guide distribute-in-house

all-pdf: user-guide-pdf troubleshooting-guide-pdf distribute-in-house-pdf

all-docx: user-guide-docx troubleshooting-guide-docx distribute-in-house-docx

user-guide: user-guide-pdf user-guide-docx

user-guide-pdf:
	test -d "$(OUTDIR)" || mkdir "$(OUTDIR)" 
	cat user-guide.md | pandoc ${PANDOC_OPT_PDF} -o "$(OUTDIR)/user-guide-$(DATE).pdf"

user-guide-docx:
	test -d "$(OUTDIR)" || mkdir "$(OUTDIR)"
	cat user-guide.md | pandoc ${PANDOC_OPT_DOCX} -o "$(OUTDIR)/user-guide-$(DATE).docx"

troubleshooting-guide: troubleshooting-guide-pdf troubleshooting-guide-docx

troubleshooting-guide-pdf:
	test -d "$(OUTDIR)" || mkdir "$(OUTDIR)" 
	cat troubleshooting-guide.md | pandoc ${PANDOC_OPT_PDF} -o "$(OUTDIR)/troubleshooting-guide-$(DATE).pdf"

troubleshooting-guide-docx:
	test -d "$(OUTDIR)" || mkdir "$(OUTDIR)"
	cat troubleshooting-guide.md | pandoc ${PANDOC_OPT_DOCX} -o "$(OUTDIR)/troubleshooting-guide-$(DATE).docx"

distribute-in-house: distribute-in-house-pdf distribute-in-house-docx

distribute-in-house-pdf:
	test -d "$(OUTDIR)" || mkdir "$(OUTDIR)" 
	cat distribute-in-house.md | pandoc ${PANDOC_OPT_PDF} -o "$(OUTDIR)/distribute-in-house-$(DATE).pdf"

distribute-in-house-docx:
	test -d "$(OUTDIR)" || mkdir "$(OUTDIR)"
	cat distribute-in-house.md | pandoc ${PANDOC_OPT_DOCX} -o "$(OUTDIR)/distribute-in-house-$(DATE).docx"

clean:
	rm -rf $(OUTDIR)
