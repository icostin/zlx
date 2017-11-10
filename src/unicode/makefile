
ucw8.c: cw.rb UnicodeData.txt EastAsianWidth.txt
	ruby cw.rb UnicodeData.txt EastAsianWidth.txt 8 > $@

UnicodeData.txt EastAsianWidth.txt:
	wget -O$@ ftp://ftp.unicode.org/Public/UNIDATA/$@

clean-unicode:
	rm -f UnicodeData.txt EastAsianWidth.txt

