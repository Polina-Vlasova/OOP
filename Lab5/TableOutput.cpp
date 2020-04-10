#include "TableOutput.h"

TableOutput::TableOutput(string name, list<list<string>> data, list<int> weights) : mWidths(weights), mName(name), mData(data)
{
	mWidth = 0;
	for (auto const &weight : mWidths)
		mWidth += weight;
}

libxl::Format* TableOutput::createFormat(libxl::Book * book)
{
	libxl::Format* format = book->addFormat();
	libxl::Font* font = book->addFont();
	font->setSize(12);
	font->setName(L"Times New Roman");
	format->setFont(font);
	format->setBorder();
	format->setAlignV(libxl::ALIGNV_CENTER);
	format->setAlignH(libxl::ALIGNH_LEFT);
	format->setFillPattern(libxl::FILLPATTERN_SOLID);
	format->setPatternForegroundColor(libxl::COLOR_WHITE);
	return format;
}

libxl::Format* TableOutput::createFormatName(libxl::Book * book)
{
	libxl::Format* format = createFormat(book);
	format->font()->setBold(true);
	format->setAlignH(libxl::ALIGNH_CENTER);
	format->setPatternForegroundColor(book->colorPack(191, 191, 191));
	return format;
}

libxl::Format* TableOutput::createFormatHeader(libxl::Book * book)
{
	libxl::Format* format = createFormat(book);
	format->font()->setItalic(true);
	format->setAlignH(libxl::ALIGNH_CENTER);
	format->setPatternForegroundColor(book->colorPack(217, 217, 217));
	return format;
}

libxl::Format* TableOutput::createFormatData(libxl::Book * book)
{
	libxl::Format* format = createFormat(book);
	format->setPatternForegroundColor(book->colorPack(242, 242, 242));
	return format;
}

void TableOutput::write(libxl::Book* book, libxl::Sheet * sheet, int& r, int& c, bool bVertical)
{
	if (mData.size() < 2)
		return;

	book->setRgbMode(true);
	libxl::Format* formatName = createFormatName(book);
	libxl::Format* formatHeader = createFormatHeader(book);
	libxl::Format* formatData = createFormatData(book);

	int row = r;
	int col = c;

	sheet->setMerge(row, row, col, col + mWidth - 1);
	for (int i = col; i < col + mWidth; i++)
		sheet->setCellFormat(row, i, formatName);
	sheet->writeStr(row, col, Converter::toWstring(mName).c_str());
	row++;

	for (auto const &dataRow : mData) {
		auto width = mWidths.begin();
		for (auto const &data : dataRow) {
			sheet->writeStr(row, col, Converter::toWstring(data).c_str());
			bool isHeader = (!bVertical && row == r + 1) || (bVertical && col == c);
			for (int i = col; i < col + *width; i++)
				sheet->setCellFormat(row, i, isHeader ? formatHeader : formatData);
			if (*width > 1) {
				sheet->setMerge(row, row, col, col + *width - 1);
			}
			col += *width;
			width++;
		}
		row++;
		col = c;
	}

	r = row;
	c = col;
}



void TableOutput::write(ostream& out, bool bVertical)
{
	if (mData.size() < 2)
		return;

	int margin = (mWidth * 22 - 1 - mName.size()) / 2 + mName.size();
	out << "|" << setw(mWidth * 22 - 1) << setfill('—') << '—' << setfill(' ') << "|" << endl;
	out << "|" << right << setw(margin) << mName << setw(mWidth * 22 - margin) << "|" << endl;

	bool isHeader = true;
	for (auto const &dataRow : mData) {
		auto width = mWidths.begin();
		out << "|" << setw(mWidth * 22 - 1) << setfill('—') << '—' << setfill(' ') << "|" << endl;
		out << "|";
		for (auto const &data : dataRow) {
			if ((dataRow == mData.front() && !bVertical) || (bVertical && data == dataRow.front())) {
				margin = (*width * 22 - data.size()) / 2 + data.size();
				out  << right << setw(margin) << data << setw(*width * 22 - margin) << "|";
			}
			else {
				out << " " << left << setw(*width * 22 - 2) << data << "|";
			}
			width++;
		}
		isHeader = false;
		out << endl;
	}
	out << "|" << setw(mWidth * 22 - 1) << setfill('—') << '—' << setfill(' ') << "|" << endl;
}

