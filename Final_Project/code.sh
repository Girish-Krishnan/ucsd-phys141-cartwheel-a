source $HOME/opt/nemo/nemo_start.sh
PATH=/Applications/glnemo2.app/Contents/MacOS/:$PATH

# Create target galaxy with magalie for just testing purposes
rm -f *.ic *.snp *.out *.log *.tab *.snp
rm -rf *.ic.tmpdir
rm -rf frames
magalie out=target_magalie.ic ndisk=8000 nbulge=2000 nhalo=30000 cdisk=5.0 hdisk=0.2 rhalo=5.0 chalo=10.0 rbulge=0.375 cbulge=10.0
snapprint in=target_magalie.ic options=m,x,y,z,vx,vy,vz > target_magalie.ic.tab
#awk '{$1 = $1 * 0.3215;}1' target_magalie.ic.tab > tmpfile.tab && mv tmpfile.tab target_magalie.ic.tab
tabtos in=target_magalie.ic.tab out=target_magalie.ic.tab.snp block1=mass,pos,vel
glnemo2 target_magalie.ic.tab.snp 0:9999

# Companion plummer has already been created
mkplummer out=companion.ic nbody=4000 scale=0.198628 rfrac=0.195

# Combining
# snapstack target_magalie.ic.tab.snp companion.ic combined_magalie.ic deltar=-8.98,9.39,18.08 deltav=1.03,-1.08,-2.07
snapstack target_magalie.ic.tab.snp companion.ic combined_magalie.ic deltar=0,0,27.68 deltav=0,0,-1.90
glnemo2 combined_magalie.ic 0:9999,40000:43999

# Running the sim and visualizing       
gyrfalcON combined_magalie.ic output_magalie.out tstop=80 step=0.05 kmax=6 eps=0.1 logfile=combined.log
glnemo2 output_magalie.out 0:9999,40000:43999 
mkdir -p frames
cd frames
glnemo2 ../output_magalie.out 0:9999,40000:43999 hsize=400 wsize=400 play=t screenshot=frame xrot=45
ffmpeg -y -i frame.%05d.jpg -c:v libx264 -vf fps=20 -pix_fmt yuv420p ../video.mp4
cd ..
python plot_energies.py