#!/bin/bash

# Script to compile the LaTeX report to PDF

echo "Compiling ImageFlow technical report..."

# Check if pdflatex is installed
if ! command -v pdflatex &> /dev/null; then
    echo "Error: pdflatex not found!"
    echo "Please install TeX Live:"
    echo "  Ubuntu/Debian: sudo apt install texlive-full"
    echo "  Fedora: sudo dnf install texlive-scheme-full"
    exit 1
fi

# Compile LaTeX document (run twice for TOC and references)
pdflatex report.tex
pdflatex report.tex

# Clean up auxiliary files
rm -f report.aux report.log report.out report.toc

echo ""
echo "✓ Compilation complete!"
echo "Output: report.pdf"
