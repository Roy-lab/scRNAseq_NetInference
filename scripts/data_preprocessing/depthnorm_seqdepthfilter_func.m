function depthnorm_seqdepthfilter_func(inputfile, outputfile, read_filter, normalize_factor);
fprintf('Depthnorming %s to %s\n',inputfile,outputfile);
d=importdata(inputfile);
e=d.data;

s=sum(e);
%filter out cell IDs that have less than 2000 total reads
goodcells = find(s > read_filter);
fprintf('Found total of %d cells\n',size(goodcells,2));
%goodcells now has the IDs of the cells we want
%We still normalize everything.
factors = s / normalize_factor;
for c=1:size(e,2)
e_dnorm(:,c)=e(:,c)/factors(c);
end

% We are going to write this as cell by genes matrix
% Let's make sure that our read depth filtering did not cause some genes to become all zeros.
genesum=sum(e_dnorm(:,goodcells),2);
badgenes=find(genesum<=0);
goodgenes=find(genesum>0);
cellnames=d.textdata(1,2:end);
genenames=d.textdata(2:end,1);
fprintf('Found %d bad genes %d good genes\n',size(badgenes,1),size(goodgenes,1));
if(size(badgenes,1)<50)
fprintf('Bad genes <50 so showing them:\n');
genenames(badgenes)'
end
[f,msg]=fopen(outputfile,'w');

assert(f>=3,msg)
fprintf(f,'Cell');
for g=1:size(goodgenes,1)
gid=goodgenes(g);
fprintf(f,'\t%s',genenames{gid});
end
fprintf(f,'\n');
%Need this for filtered cells only
for c=1:size(goodcells,2)
	cid=goodcells(c);
	fprintf(f,'%s',cellnames{cid});
	for g=1:size(goodgenes,1)
		gid=goodgenes(g);
		fprintf(f,'\t%f',sqrt(e_dnorm(gid,cid)));
	end
	fprintf(f,'\n');
end
fclose(f);	
