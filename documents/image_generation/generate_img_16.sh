#!/bin/bash
for file_name in $( find . -name img_16.tex )
do
  file_name_base=${file_name//.tex/};
  pdf_file_name=$(eval "echo $file_name_base.pdf");
  pdf_file_name=${pdf_file_name//\/tex/}
  png_file_name=$(eval "echo $file_name_base.png");
  png_file_name=${png_file_name//\/tex/\/img}

  echo "Building" $pdf_file_name "..."

  pdflatex $file_name;

  echo "Building" $pdf_file_name "successfully finished!"
  echo "Generating" $png_file_name ".."

  convert -density 800 $pdf_file_name -quality 100 $png_file_name

  echo "Generating" $png_file_name "successfully finished!"
done

echo "Clearing *.aux, *.log, *.pdf files...";
rm -r *.aux *.log *.pdf
