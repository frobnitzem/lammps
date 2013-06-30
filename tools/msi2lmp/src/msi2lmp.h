/********************************
*
* Header file for msi2lmp conversion program.
*
* This is the header file for the third version of a program
* that generates a LAMMPS data file based on the information
* in an MSI car file (atom coordinates) and mdf file (molecular
* topology). A key part of the program looks up forcefield parameters
* from an MSI frc file.
*
* The first version was written by Steve Lustig at Dupont, but
* required using Discover to derive internal coordinates and
* forcefield parameters
*
* The second version was written by Michael Peachey while an
* intern in the Cray Chemistry Applications Group managed
* by John Carpenter. This version derived internal coordinates
* from the mdf file and looked up parameters in the frc file
* thus eliminating the need for Discover.
*
* The third version was written by John Carpenter to optimize
* the performance of the program for large molecular systems
* (the original code for derving atom numbers was quadratic in time)
* and to make the program fully dynamic. The second version used
* fixed dimension arrays for the internal coordinates.
*
* John Carpenter can be contacted by sending email to
* jec374@earthlink.net
*
* November 2000
*
*
*/

# include <stdio.h>

#define PI_180  0.01745329251994329576

#define MAX_LINE_LENGTH  256
#define MAX_CONNECTIONS    8
#define MAX_STRING        64
#define MAX_NAME          16

#define MAX_ATOM_TYPES         100
#define MAX_BOND_TYPES         200
#define MAX_ANGLE_TYPES        300
#define MAX_DIHEDRAL_TYPES     400
#define MAX_OOP_TYPES          400
#define MAX_ANGLEANGLE_TYPES   400
#define MAX_TYPES            12000

struct ResidueList {
  int start;
  int end;
  char name[MAX_NAME];
};

struct MoleculeList {
  int start;
  int end;
  int no_residues;
  struct ResidueList *residue;
};

/* Internal coodinate Lists */

struct BondList {
  int type;
  int members[2];
};

struct AngleList {
  int type;
  int members[3];
};

struct DihedralList {
  int type;
  int members[4];
};

struct OOPList {
  int type;
  int members[4];
};

struct AngleAngleList {
  int type;
  int members[4];
};

/* Internal coodinate Types Lists */


struct AtomTypeList
{
  char potential[5];
  float mass;
  double params[2];
  int no_connect;
};

struct BondTypeList {
  int types[2];
  double params[4];
};

struct AngleTypeList {
  int types[3];
  double params[4];
  double bondangle_cross_term[4];
  double bondbond_cross_term[3];
};

struct DihedralTypeList {
  int types[4];
  double params[6];
  double endbonddihedral_cross_term[8];
  double midbonddihedral_cross_term[4];
  double angledihedral_cross_term[8];
  double angleangledihedral_cross_term[3];
  double bond13_cross_term[3];
};

struct OOPTypeList {
  int types[4];
  double params[3];
  double angleangle_params[6];
};

struct AngleAngleTypeList {
  int types[4];
  double params[6];
};

/* ---------------------------------------------- */

struct Atom {
  int   molecule;        /* molecule id */
  int   no;              /* atom id */
  char  name[MAX_NAME];  /* atom name */
  double x[3];           /* position vector */
  char  potential[6];    /* atom potential type */
  char  element[4];      /* atom element */
  float q;               /* charge */
  char  residue_string[MAX_NAME]; /* residue string */
  int  no_connect;	/* number of connections to atom */
  char connections[MAX_CONNECTIONS][MAX_STRING];  /* long form, connection name*/
  double bond_order[MAX_CONNECTIONS];
  int conn_no[MAX_CONNECTIONS];	 /* Atom number to which atom is connected */
  int type;
};

extern char  *rootname;
extern char  *FrcFileName;
extern double pbc[9];
extern int    periodic;      /* 0= nonperiodic 1= 3-D periodic */
extern int    TriclinicFlag; /* 0= Orthogonal  1= Triclinic */
extern int    forcefield;    /* 1= ClassI      2= ClassII */
extern int    pflag;         /* print level: 0, 1, 2, 3 */
extern int    iflag;         /* 0 stop at errors   1 = ignore errors */
extern int    *no_atoms;
extern int    no_molecules;
extern int    replicate[3];
extern int    total_no_atoms;
extern int    total_no_bonds;
extern int    total_no_angles;
extern int    total_no_dihedrals;
extern int    total_no_angle_angles;
extern int    total_no_oops;
extern int    no_atom_types;
extern int    no_bond_types;
extern int    no_angle_types;
extern int    no_dihedral_types;
extern int    no_oop_types;
extern int    no_angleangle_types;
extern FILE   *CarF;
extern FILE   *FrcF;
extern FILE   *PrmF;
extern FILE   *MdfF;
extern FILE   *RptF;
extern struct Atom *atoms;
extern struct MoleculeList *molecule;
extern struct BondList *bonds;
extern struct AngleList *angles;
extern struct DihedralList *dihedrals;
extern struct OOPList *oops;
extern struct AngleAngleList *angleangles;
extern struct AtomTypeList *atomtypes;
extern struct BondTypeList *bondtypes;
extern struct AngleTypeList *angletypes;
extern struct DihedralTypeList *dihedraltypes;
extern struct OOPTypeList *ooptypes;
extern struct AngleAngleTypeList *angleangletypes;

extern void FrcMenu();
extern void ReadCarFile();
extern void ReadMdfFile();
extern void ReadFrcFile();
extern void MakeLists();
extern void GetParameters(int);
extern void CheckLists();
extern void WriteDataFile01(char *,int);
extern void WriteDataFile05(char *,int);
