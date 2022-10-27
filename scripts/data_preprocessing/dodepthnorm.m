% change indir to directory of input file
indir='indir/'
% change outdir to directory output file will be written to
outdir='depth_normalized/'
% change fnames to prefix of datafiel
fnames={'gasch';'shalek'};

for f=1:2

	infname=sprintf('%s/%s_intersect.txt',indir,fnames{f});
	outfname=sprintf('%s/%s_depthnorm_seqdepthfilter.txt',outdir,fnames{f});
	depthnorm_seqdepthfilter_func(infname,outfname);
end

