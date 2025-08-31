/*-------------------------------------------------------------------------*/
**GitBash Push Commands**
/*-------------------------------------------------------------------------*/
cd /C/Users/centa/OneDrive/Documents/Unreal/TOTU_5.5
git add .
git commit -m "TOTU v2.05 (6.3.2025)"
git push origin main
/*-------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
**GitBash Pull Commands**
/*-------------------------------------------------------------------------*/
cd /C/Users/centa/Documents/Unreal/TOTU
git fetch origin
git pull origin main

**(use for avoiding merge conflicts)**
git pull --ff-only origin main

**(use for discarding local changes and fetching GitHub files)**
git fetch origin
git reset --hard origin/main
/*-------------------------------------------------------------------------*/
