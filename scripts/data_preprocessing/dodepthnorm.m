indir='/mnt/dv/wid/projects2/Roy-regnet-inference/singlecell/sahalab/data_SR/filtered'
outdir='/mnt/dv/wid/projects2/Roy-regnet-inference/singlecell/sahalab/data/depth_normalized'
fnames={'anemone';'daisy';'lilac';'lotus';'magnolia';'marigold';'orchid';'tulip';'zinnia'};

for f=1:9

	infname=sprintf('%s/%s_intersect.txt',indir,fnames{f});
	%outfname=sprintf('%s/%s_depthnorm_t.txt',indir,fnames{f});
	
	%depthnorm_func(infname,outfname);
	%outfname=sprintf('%s/%s_depthnorm_sqrt_t.txt',indir,fnames{f});
	%depthnorm_func_sqrt(infname,outfname);
	outfname=sprintf('%s/%s_depthnorm_seqdepthfilter.txt',outdir,fnames{f});
	depthnorm_seqdepthfilter_func(infname,outfname);
end

