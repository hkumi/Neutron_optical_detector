#include "construction.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include <algorithm>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
{

    DefineMaterials();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{

  G4NistManager *nist = G4NistManager::Instance();
  G4int ncomponents, natoms;
  G4double massfraction;

  G4double Vdens = 1.e-25*g/cm3;
  G4double Vpres = 1.e-19*pascal;
  G4double Vtemp = 0.1*kelvin;
  
  G4double a, z;
  C = nist->FindOrBuildElement("C");
  N  = new G4Element("Nitrogen","N",7.,14.007*g/mole);
  O  = new G4Element("Oxygen","O",8.,15.999*g/mole);
  F  = new G4Element("Fluorine","F",9.,18.998*g/mole);

   // pressurized water
  G4Element* H  = new G4Element("TS_H_of_Water" ,"H" , 1., 1.0079*g/mole);
  G4Material* H2O = 
  new G4Material("Water_ts", 1.000*g/cm3, ncomponents=2,
                         kStateLiquid, 593*kelvin, 150*bar);
  H2O->AddElement(H, natoms=2);
  H2O->AddElement(O, natoms=1);
  H2O->GetIonisation()->SetMeanExcitationEnergy(78.0*eV);
  // vacuum
  Vacc = new G4Material("Galactic", z=1, a=1.01*g/mole, Vdens, kStateGas, Vtemp, Vpres);
  // air
  G4Element* N = new G4Element("Nitrogen", "N", 7., 14.01*g/mole);
  Air = new G4Material("air", 1.290*mg/cm3, ncomponents=2, kStateGas, 293*kelvin, 1*atmosphere);
  Air->AddElement(N, massfraction=70.*perCent);
  Air->AddElement(O, massfraction=30.*perCent);

  // polyethilene
  G4Element* Hpe = new G4Element("TS_H_of_Polyethylene", "H", 1, 1.0079*g/mole);
  G4Element* Cpe = new G4Element("Carbon", "C", 6, 12.01*g/mole);
  polyethylene = new G4Material("polyethylene", 0.93*g/cm3, ncomponents=2, kStateSolid, 293*kelvin, 1*atmosphere);
  polyethylene->AddElement(Hpe, natoms=4);
  polyethylene->AddElement(Cpe, natoms=2);

  //silicon_detector material
  nist->FindOrBuildMaterial("G4_Si");
  siliconMaterial = G4Material::GetMaterial("G4_Si");//G4Material::GetMaterial("G4_Si");
 //...................................creating the optical detector material ...................................
//----------------------------------- CarbonTetrafluoride ------------------------
  G4double pressure = 0.0328947*atmosphere; //25Torr
  G4double temperature = 293.15*kelvin; // 
  CF4 = new G4Material("CF4", 0.1223*mg/cm3,2,kStateGas,temperature,pressure);
  CF4->AddElement(C,1);
  CF4->AddElement(F,4);

  const G4int iNbEntries = 300;

  G4double CF4PhotonMomentum[iNbEntries] = {6.2*eV,6.138613861*eV,6.078431373*eV,6.019417476*eV,5.961538462*eV,5.904761905*eV,5.849056604*eV,5.794392523*eV,5.740740741*eV,5.688073394*eV,5.636363636*eV,5.585585586*eV,
						5.535714286*eV,5.486725664*eV,5.438596491*eV,5.391304348*eV,5.344827586*eV,5.299145299*eV,5.254237288*eV,5.210084034*eV,5.166666667*eV,5.123966942*eV,5.081967213*eV,5.040650407*eV,5*eV,4.96*eV,4.920634921*eV,
						4.881889764*eV,4.84375*eV,4.80620155*eV,4.769230769*eV,4.732824427*eV,4.696969697*eV,4.661654135*eV,4.626865672*eV,4.592592593*eV,4.558823529*eV,4.525547445*eV,4.492753623*eV,4.460431655*eV,
						4.428571429*eV,4.397163121*eV,4.366197183*eV,4.335664336*eV,4.305555556*eV,4.275862069*eV,4.246575342*eV,4.217687075*eV,4.189189189*eV,4.161073826*eV,4.133333333*eV,4.105960265*eV,4.078947368*eV,4.052287582*eV,
						4.025974026*eV,4*eV,3.974358974*eV,3.949044586*eV,3.924050633*eV,3.899371069*eV,3.875*eV,3.850931677*eV,3.827160494*eV,3.803680982*eV,3.780487805*eV,3.757575758*eV,3.734939759*eV,
						3.71257485*eV,3.69047619*eV,3.668639053*eV,3.647058824*eV,3.625730994*eV,3.604651163*eV,3.583815029*eV,3.563218391*eV,3.542857143*eV,3.522727273*eV,3.502824859*eV,3.483146067*eV,
						3.463687151*eV,3.444444444*eV,3.425414365*eV,3.406593407*eV,3.387978142*eV,3.369565217*eV,3.351351351*eV,3.333333333*eV,3.315508021*eV,3.29787234*eV,3.28042328*eV,3.263157895*eV,
						3.246073298*eV,3.229166667*eV,3.212435233*eV,3.195876289*eV,3.179487179*eV,3.163265306*eV,3.147208122*eV,3.131313131*eV,3.115577889*eV,3.1*eV,3.084577114*eV,3.069306931*eV,3.054187192*eV,
						3.039215686*eV,3.024390244*eV,3.009708738*eV,2.995169082*eV,2.980769231*eV,2.966507177*eV,2.952380952*eV,2.938388626*eV,2.924528302*eV,2.910798122*eV,2.897196262*eV,2.88372093*eV,
						2.87037037*eV,2.857142857*eV,2.844036697*eV,2.831050228*eV,2.818181818*eV,2.805429864*eV,2.792792793*eV,2.780269058*eV,2.767857143*eV,2.755555556*eV,2.743362832*eV,
						2.731277533*eV,2.719298246*eV,2.707423581*eV,2.695652174*eV,2.683982684*eV,2.672413793*eV,2.660944206*eV,2.64957265*eV,2.638297872*eV,2.627118644*eV,2.616033755*eV,2.605042017*eV,
						2.594142259*eV,2.583333333*eV,2.572614108*eV,2.561983471*eV,2.551440329*eV,2.540983607*eV,2.530612245*eV,2.520325203*eV,2.510121457*eV,2.5*eV,2.489959839*eV,2.48*eV,2.470119522*eV,2.46031746*eV,
						2.450592885*eV,2.440944882*eV,2.431372549*eV,2.421875*eV,2.412451362*eV,2.403100775*eV,2.393822394*eV,2.384615385*eV,2.375478927*eV,2.366412214*eV,2.357414449*eV,
						2.348484848*eV,2.339622642*eV,2.330827068*eV,2.322097378*eV,2.313432836*eV,2.304832714*eV,2.296296296*eV,2.287822878*eV,2.279411765*eV,2.271062271*eV,2.262773723*eV,2.254545455*eV,2.246376812*eV,2.238267148*eV,
						2.230215827*eV,2.222222222*eV,2.214285714*eV,2.206405694*eV,2.19858156*eV,2.190812721*eV,2.183098592*eV,2.175438596*eV,2.167832168*eV,2.160278746*eV,2.152777778*eV,2.14532872*eV,2.137931034*eV,2.130584192*eV,
						2.123287671*eV,2.116040956*eV,2.108843537*eV,2.101694915*eV,2.094594595*eV,2.087542088*eV,2.080536913*eV,2.073578595*eV,2.066666667*eV,2.059800664*eV,2.052980132*eV,2.04620462*eV,2.039473684*eV,2.032786885*eV,
						2.026143791*eV,2.019543974*eV,2.012987013*eV,2.006472492*eV,2*eV,1.993569132*eV,1.987179487*eV,1.980830671*eV,1.974522293*eV,1.968253968*eV,1.962025316*eV,1.955835962*eV,
						1.949685535*eV,1.943573668*eV,1.9375*eV,1.931464174*eV,1.925465839*eV,1.919504644*eV,1.913580247*eV,1.907692308*eV,1.901840491*eV,1.896024465*eV,1.890243902*eV,1.88449848*eV,1.878787879*eV,1.873111782*eV,
						1.86746988*eV,1.861861862*eV,1.856287425*eV,1.850746269*eV,1.845238095*eV,1.839762611*eV,1.834319527*eV,1.828908555*eV,1.823529412*eV,1.818181818*eV,
						1.812865497*eV,1.807580175*eV,1.802325581*eV,1.797101449*eV,1.791907514*eV,1.786743516*eV,1.781609195*eV,1.776504298*eV,1.771428571*eV,1.766381766*eV,1.761363636*eV,1.756373938*eV,1.751412429*eV,
						1.746478873*eV,1.741573034*eV,1.736694678*eV,1.731843575*eV,1.727019499*eV,1.722222222*eV,1.717451524*eV,1.712707182*eV,1.707988981*eV,1.703296703*eV,1.698630137*eV,1.693989071*eV,1.689373297*eV,1.684782609*eV,
						1.680216802*eV,1.675675676*eV,1.67115903*eV,1.666666667*eV,1.662198391*eV,1.657754011*eV,1.653333333*eV,1.64893617*eV,1.644562334*eV,1.64021164*eV,1.635883905*eV,1.631578947*eV,1.627296588*eV,1.623036649*eV,
						1.618798956*eV,1.614583333*eV,1.61038961*eV,1.606217617*eV,1.602067183*eV,1.597938144*eV,1.593830334*eV,1.58974359*eV,1.585677749*eV,1.581632653*eV,1.577608142*eV,1.573604061*eV,1.569620253*eV,
						1.565656566*eV,1.561712846*eV,1.557788945*eV,1.553884712*eV};

  // Sort the array in ascending order
  std::sort(CF4PhotonMomentum, CF4PhotonMomentum + iNbEntries);

 G4double CF4Scintillation_Fast[iNbEntries]    = {0.0029,0.0029,0.0017,0.0024,0.0018,0.0011,0.0027,0.0009,0.0003,0.0019,0.0030,0.0024,0.0023,0.0036,0.0039,0.0056,
						0.0049,0.0061,0.0053,0.0052,0.0056,0.0064,0.0072,0.0064,0.0080,0.0071,0.0056,0.0069,0.0053,0.0070,0.0060,0.0057,0.0071,0.0066,0.0066,
						0.0055,0.0082,0.0076,0.0093,0.0089,0.0106,0.0109,0.0105,0.0102,0.0120,0.0121,0.0102,0.0097,0.0120,0.0126,0.0097,0.0103,0.0097,0.0084,
						0.0119,0.0112,0.0096,0.0171,0.0235,0.0078,0.0089,0.0071,0.0065,0.0074,0.0073,0.0074,0.0074,0.0080,0.0143,0.0522,0.0069,0.0076,0.0042,
						0.0059,0.0039,0.0053,0.0054,0.0185,0.0077,0.0599,0.0048,0.0034,0.0041,0.0041,0.0047,0.0059,0.0046,0.0065,0.0128,0.0037,0.0167,0.0053,
						0.0038,0.0042,0.0046,0.0032,0.0037,0.0073,0.0049,0.0067,0.0116,0.0054,0.0077,0.0111,0.0042,0.0043,0.0037,0.0046,0.0041,0.0028,0.0055,
						0.0031,0.0048,0.0057,0.0056,0.0035,0.0039,0.0068,0.0051,0.0037,0.0054,0.0048,0.0061,0.0033,0.0050,0.0052,0.0047,0.0014,0.0043,0.0041,
						0.0023,0.0062,0.0036,0.0038,0.0039,0.0043,0.0049,0.0049,0.0036,0.0048,0.0039,0.0023,0.0035,0.0025,0.0036,0.0010,0.0044,0.0013,0.0041,
						0.0021,0.0016,0.0046,0.0040,0.0034,0.0027,0.0026,0.0034,0.0004,0.0037,0.0004,0.0036,0.0029,0.0029,0.0036,0.0055,0.0034,0.0034,0.0025,
						0.0028,0.0055,0.0064,0.0037,0.0029,0.0047,0.0058,0.0040,0.0062,0.0055,0.0029,0.0067,0.0070,0.0080,0.0060,0.0094,0.0082,0.0072,0.0089,
						0.0117,0.0102,0.0134,0.0131,0.0131,0.0120,0.0135,0.0096,0.0107,0.0179,0.0210,0.0172,0.0165,0.0167,0.0176,0.0137,0.0196,0.0217,0.0175,
						0.0223,0.0192,0.0222,0.0188,0.0184,0.0183,0.0156,0.0098,0.0198,0.0268,0.0188,0.0236,0.0208,0.0171,0.0229,0.0228,0.0227,0.0204,0.0184,
						0.0190,0.0185,0.0145,0.0138,0.0122,0.0180,0.0132,0.0146,0.0087,0.0039,0.0147,0.0000,0.0000,0.0137,0.0084,0.0094,0.0114,0.0078,0.0100,
						0.0069,0.0055,0.0164,0.0113,0.0148,0.0053,0.0054,0.0065,0.0092,0.0000,0.0047,0.0000,0.0071,0.0000,0.0057,0.0063,0.0064,0.0050,0.0077,
						0.0034,0.0025,0.0000,0.0041,0.0025,0.0019,0.0042,0.0030,0.0000,0.0030,0.0000,0.0000,0.0000,0.0027,0.0000,0.0000,0.0000,0.0000,0.0006,
						0.0051,0.0083,0.0000,0.0000,0.0064,0.0003,0.0002,0.0074,0.0038,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000};

  std::sort(CF4Scintillation_Fast, CF4Scintillation_Fast + iNbEntries);  
  G4double CF4Scintillation_Slow[iNbEntries]    = {0.0029,0.0029,0.0017,0.0024,0.0018,0.0011,0.0027,0.0009,0.0003,0.0019,0.0030,0.0024,0.0023,0.0036,0.0039,0.0056,
						0.0049,0.0061,0.0053,0.0052,0.0056,0.0064,0.0072,0.0064,0.0080,0.0071,0.0056,0.0069,0.0053,0.0070,0.0060,0.0057,0.0071,0.0066,0.0066,
						0.0055,0.0082,0.0076,0.0093,0.0089,0.0106,0.0109,0.0105,0.0102,0.0120,0.0121,0.0102,0.0097,0.0120,0.0126,0.0097,0.0103,0.0097,0.0084,
						0.0119,0.0112,0.0096,0.0171,0.0235,0.0078,0.0089,0.0071,0.0065,0.0074,0.0073,0.0074,0.0074,0.0080,0.0143,0.0522,0.0069,0.0076,0.0042,
						0.0059,0.0039,0.0053,0.0054,0.0185,0.0077,0.0599,0.0048,0.0034,0.0041,0.0041,0.0047,0.0059,0.0046,0.0065,0.0128,0.0037,0.0167,0.0053,
						0.0038,0.0042,0.0046,0.0032,0.0037,0.0073,0.0049,0.0067,0.0116,0.0054,0.0077,0.0111,0.0042,0.0043,0.0037,0.0046,0.0041,0.0028,0.0055,
						0.0031,0.0048,0.0057,0.0056,0.0035,0.0039,0.0068,0.0051,0.0037,0.0054,0.0048,0.0061,0.0033,0.0050,0.0052,0.0047,0.0014,0.0043,0.0041,
						0.0023,0.0062,0.0036,0.0038,0.0039,0.0043,0.0049,0.0049,0.0036,0.0048,0.0039,0.0023,0.0035,0.0025,0.0036,0.0010,0.0044,0.0013,0.0041,
						0.0021,0.0016,0.0046,0.0040,0.0034,0.0027,0.0026,0.0034,0.0004,0.0037,0.0004,0.0036,0.0029,0.0029,0.0036,0.0055,0.0034,0.0034,0.0025,
						0.0028,0.0055,0.0064,0.0037,0.0029,0.0047,0.0058,0.0040,0.0062,0.0055,0.0029,0.0067,0.0070,0.0080,0.0060,0.0094,0.0082,0.0072,0.0089,
						0.0117,0.0102,0.0134,0.0131,0.0131,0.0120,0.0135,0.0096,0.0107,0.0179,0.0210,0.0172,0.0165,0.0167,0.0176,0.0137,0.0196,0.0217,0.0175,
						0.0223,0.0192,0.0222,0.0188,0.0184,0.0183,0.0156,0.0098,0.0198,0.0268,0.0188,0.0236,0.0208,0.0171,0.0229,0.0228,0.0227,0.0204,0.0184,
						0.0190,0.0185,0.0145,0.0138,0.0122,0.0180,0.0132,0.0146,0.0087,0.0039,0.0147,0.0000,0.0000,0.0137,0.0084,0.0094,0.0114,0.0078,0.0100,
						0.0069,0.0055,0.0164,0.0113,0.0148,0.0053,0.0054,0.0065,0.0092,0.0000,0.0047,0.0000,0.0071,0.0000,0.0057,0.0063,0.0064,0.0050,0.0077,
						0.0034,0.0025,0.0000,0.0041,0.0025,0.0019,0.0042,0.0030,0.0000,0.0030,0.0000,0.0000,0.0000,0.0027,0.0000,0.0000,0.0000,0.0000,0.0006,
						0.0051,0.0083,0.0000,0.0000,0.0064,0.0003,0.0002,0.0074,0.0038,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000,0.0000};

   std::sort(CF4Scintillation_Slow, CF4Scintillation_Slow + iNbEntries);

  const G4int iNbEntries_1 = 3;
  G4double CF4PhotonMomentum_1[iNbEntries_1] = {200*eV,500*eV,700*eV};
  G4double CF4RefractiveIndex[iNbEntries_1]  = {1.004,1.004,1.004};
  G4double CF4AbsorbtionLength[iNbEntries_1] = {100.*cm, 100.*cm, 100.*cm};
  G4double CF4ScatteringLength[iNbEntries_1] = {30.*cm,  30.*cm,  30.*cm};
  G4MaterialPropertiesTable *CF4PropertiesTable = new G4MaterialPropertiesTable();
  CF4PropertiesTable->AddProperty("FASTCOMPONENT", CF4PhotonMomentum, CF4Scintillation_Fast, iNbEntries,true);
  CF4PropertiesTable->AddProperty("SLOWCOMPONENT", CF4PhotonMomentum, CF4Scintillation_Slow, iNbEntries,true);
  CF4PropertiesTable->AddProperty("RINDEX", CF4PhotonMomentum_1, CF4RefractiveIndex, iNbEntries_1);
  CF4PropertiesTable->AddProperty("ABSLENGTH", CF4PhotonMomentum_1, CF4AbsorbtionLength, iNbEntries_1);
  CF4PropertiesTable->AddProperty("RAYLEIGH", CF4PhotonMomentum_1, CF4ScatteringLength, iNbEntries_1);
  CF4PropertiesTable->AddConstProperty("SCINTILLATIONYIELD", 2500./keV,true);  // for electron recoil
  CF4PropertiesTable->AddConstProperty("RESOLUTIONSCALE", 1.0);
  CF4PropertiesTable->AddConstProperty("FASTTIMECONSTANT", 3.*ns,true);
  CF4PropertiesTable->AddConstProperty("SLOWTIMECONSTANT", 10.*ns,true);
  CF4PropertiesTable->AddConstProperty("YIELDRATIO", 1.0,true);
  CF4->SetMaterialPropertiesTable(CF4PropertiesTable);
  siliconMaterial->SetMaterialPropertiesTable(CF4PropertiesTable);
//:....................................................................................
  G4Material *SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2);
   Aerogel = new G4Material("Aerogel", 0.200*g/cm3, 3);
  SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
  SiO2->AddElement(nist->FindOrBuildElement("O"), 2);
  Aerogel->AddMaterial(SiO2, 62.5*perCent);
  Aerogel->AddMaterial(H2O, 37.4*perCent);
  Aerogel->AddElement(C, 0.1*perCent);

  G4double energy[2] = {1.239841939*eV/0.9, 1.239841939*eV/0.2};
  G4double rindexAerogel[2] = {1.1, 1.1};
  G4double rindexWorld[2] = {1.0, 1.0};
  G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
  mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2);
  Aerogel->SetMaterialPropertiesTable(mptAerogel);
  G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
  mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);
  Air->SetMaterialPropertiesTable(mptWorld);

 //.....................teflon............................

//......................end of teflon.....................

  //............................End of optical detector material........................

//...............creating the materials for the scintillator..............................
  Na = nist->FindOrBuildElement("Na");
  I = nist->FindOrBuildElement("I");
  NaI = new G4Material("NaI", 3.67*g/cm3, 2);
  NaI->AddElement(Na, 1);
  NaI->AddElement(I, 1);



//....................End of scintillator material........................................
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::ConstructOPPAC_1(G4double Pos_PPAC_1)
{

// scores
  G4double ScThick_1 =  3.0*mm;

  auto sScore_1 = new G4Box("sScore_1",
                            50/2*mm,50/2*mm,ScThick_1/2);

  auto fLScore_1 = new G4LogicalVolume(sScore_1,
                                       CF4,
                                      "fLScore_1");

  auto fPScore_r_1 = new G4PVPlacement(0,
                                    G4ThreeVector(0.*mm,0.*mm,Pos_PPAC_1),
                                    fLScore_1,
                                    "fPScore_r_1",
                                    fLBox,
                                    false,
                                    0,true);
  fScoringVolume_1 = fLScore_1;
}

void DetectorConstruction::ConstructOPPAC_2(G4double Pos_PPAC_2)
{

// scores
  G4double ScThick_2 =  3.0*mm;

  auto sScore_2 = new G4Box("sScore_2",
                            50/2*mm,50/2*mm,ScThick_2/2);

  auto fLScore_2 = new G4LogicalVolume(sScore_2,
                                       CF4,
                                      "fLScore_2");

  auto fPScore_r_2 = new G4PVPlacement(0,
                                    G4ThreeVector(0.*mm,0.*mm,Pos_PPAC_2),
                                    fLScore_2,
                                    "fPScore_r_2",
                                    fLBox,
                                    false,
                                    0,true);
  fScoringVolume_2 = fLScore_2;
}

void DetectorConstruction::ConstructOPPAC_3(G4double Pos_PPAC_3)
{

// scores
  G4double ScThick_3 =  3.0*mm;

  auto sScore_3 = new G4Box("sScore_3",
                            50/2*mm,50/2*mm,ScThick_3/2);

  auto fLScore_3 = new G4LogicalVolume(sScore_3,
                                       CF4,
                                      "fLScore_3");

  auto fPScore_r_3 = new G4PVPlacement(0,
                                    G4ThreeVector(0.*mm,0.*mm,Pos_PPAC_3),
                                    fLScore_3,
                                    "fPScore_r_3",
                                    fLBox,
                                    false,
                                    0,true);
  fScoringVolume_3 = fLScore_3;
}

void DetectorConstruction::ConstructOPPAC_4(G4double Pos_PPAC_4)
{

// scores
  G4double ScThick_4 =  3.0*mm;

  auto sScore_4 = new G4Box("sScore_4",
                            50/2*mm,50/2*mm,ScThick_4/2);

  auto fLScore_4 = new G4LogicalVolume(sScore_4,
                                       CF4,
                                      "fLScore_4");

  auto fPScore_r_4 = new G4PVPlacement(0,
                                    G4ThreeVector(0.*mm,0.*mm,Pos_PPAC_4),
                                    fLScore_4,
                                    "fPScore_r_4",
                                    fLBox,
                                    false,
                                    0,true);
  fScoringVolume_4 = fLScore_4;
}


void DetectorConstruction::ConstructOPPAC_5(G4double Pos_PPAC_5)
{

// scores
  G4double ScThick_5 =  3.0*mm;

  auto sScore_5 = new G4Box("sScore_5",
                            50/2*mm,50/2*mm,ScThick_5/2);

  auto fLScore_5 = new G4LogicalVolume(sScore_5,
                                       CF4,
                                      "fLScore_5");

  auto fPScore_r_5 = new G4PVPlacement(0,
                                    G4ThreeVector(0.*mm,0.*mm,Pos_PPAC_5),
                                    fLScore_5,
                                    "fPScore_r_5",
                                    fLBox,
                                    false,
                                    0,true);
  fScoringVolume_5 = fLScore_5;
}

void DetectorConstruction::ConstructOPPAC_6(G4double Pos_PPAC_6)
{

// scores
  G4double ScThick_6 =  3.0*mm;

  auto sScore_6 = new G4Box("sScore_6",
                            50/2*mm,50/2*mm,ScThick_6/2);

  auto fLScore_6 = new G4LogicalVolume(sScore_6,
                                       CF4,
                                      "fLScore_6");

  auto fPScore_r_6 = new G4PVPlacement(0,
                                    G4ThreeVector(0.*mm,0.*mm,Pos_PPAC_6),
                                    fLScore_6,
                                    "fPScore_r_6",
                                    fLBox,
                                    false,
                                    0,true);
  fScoringVolume_6 = fLScore_6;
}

void DetectorConstruction::ConstructOPPAC_7(G4double Pos_PPAC_7)
{

// scores
  G4double ScThick_7 =  3.0*mm;

  auto sScore_7 = new G4Box("sScore_7",
                            50/2*mm,50/2*mm,ScThick_7/2);

  auto fLScore_7 = new G4LogicalVolume(sScore_7,
                                       CF4,
                                      "fLScore_7");

  auto fPScore_r_7 = new G4PVPlacement(0,
                                    G4ThreeVector(0.*mm,0.*mm,Pos_PPAC_7),
                                    fLScore_7,
                                    "fPScore_r_7",
                                    fLBox,
                                    false,
                                    0,true);
  fScoringVolume_7 = fLScore_7;
}

void DetectorConstruction::ConstructOPPAC_8(G4double Pos_PPAC_8)
{

// scores
  G4double ScThick_8 =  3.0*mm;

  auto sScore_8 = new G4Box("sScore_8",
                            50/2*mm,50/2*mm,ScThick_8/2);

  auto fLScore_8 = new G4LogicalVolume(sScore_8,
                                       CF4,
                                      "fLScore_8");

  auto fPScore_r_8 = new G4PVPlacement(0,
                                    G4ThreeVector(0.*mm,0.*mm,Pos_PPAC_8),
                                    fLScore_8,
                                    "fPScore_r_8",
                                    fLBox,
                                    false,
                                    0,true);
  fScoringVolume_8 = fLScore_8;
}

void DetectorConstruction::ConstructOPPAC_9(G4double Pos_PPAC_9)
{

// scores
  G4double ScThick_9 =  3.0*mm;

  auto sScore_9 = new G4Box("sScore_9",
                            50/2*mm,50/2*mm,ScThick_9/2);

  auto fLScore_9 = new G4LogicalVolume(sScore_9,
                                       CF4,
                                      "fLScore_9");

  auto fPScore_r_9 = new G4PVPlacement(0,
                                    G4ThreeVector(0.*mm,0.*mm,Pos_PPAC_9),
                                    fLScore_9,
                                    "fPScore_r_9",
                                    fLBox,
                                    false,
                                    0,true);
  fScoringVolume_9 = fLScore_9;

}

void DetectorConstruction::ConstructOPPAC_10(G4double Pos_PPAC_10)
{

// scores
  G4double ScThick_10 =  3.0*mm;

  auto sScore_10 = new G4Box("sScore_10",
                            50/2*mm,50/2*mm,ScThick_10/2);

  auto fLScore_10 = new G4LogicalVolume(sScore_10,
                                       CF4,
                                      "fLScore_10");

  auto fPScore_r_10 = new G4PVPlacement(0,
                                    G4ThreeVector(0.*mm,0.*mm,Pos_PPAC_10),
                                    fLScore_10,
                                    "fPScore_r_10",
                                    fLBox,
                                    false,
                                    0,true);
  fScoringVolume_10 = fLScore_10;
}



void DetectorConstruction::ConstructSilicon_detector(G4double Pos_Silicon)
{

// scores
  G4double SiliconThick =  3.0*mm;

  auto siliconbox = new G4Box("siliconbox",
                            25/2*mm,25/2*mm,SiliconThick);

  auto siliconLV = new G4LogicalVolume(siliconbox,
                                       siliconMaterial,
                                      "siliconLV");

  auto siliconPV = new G4PVPlacement(0,
                                    G4ThreeVector(0.*mm,0.*mm,Pos_Silicon),
                                    siliconLV,
                                    "siliconPV",
                                    fLBox,
                                    false,
                                    0,true);
  fScoringVolume2 = siliconLV;
}


void DetectorConstruction::CreateAndPlaceShield(G4double thickness, G4double size, G4double position, G4LogicalVolume* motherVolume) {
    G4Box* shield = new G4Box("shield", size, size, thickness);
    G4LogicalVolume* lShield = new G4LogicalVolume(shield, polyethylene, "Shield");
    
    G4PVPlacement* pShield = new G4PVPlacement(0,
                                               G4ThreeVector(0.*mm, 0.*mm, position),
                                               lShield,
                                               "Shield",
                                               motherVolume,
                                               false,
                                               0,true);
}

void DetectorConstruction::Sphereball( G4double position) {
     G4double minSphereradius =  0*mm; 
     G4double maxSphereradius = 30*mm; 
     G4Sphere* sphereball = new G4Sphere("sphereball", minSphereradius/2, maxSphereradius/2 , 0*deg,360*deg,0*deg,180*deg);
     G4LogicalVolume* sphereVolume = new G4LogicalVolume(sphereball, polyethylene, "Sphere");
     G4PVPlacement* spherePlacement  = new G4PVPlacement(0,
                                               G4ThreeVector(0.*mm, 0.*mm, position),
                                               sphereVolume,
                                               "Sphere",
                                               fLBox,
                                               false,
                                               0,true);
     G4VisAttributes* blue = new G4VisAttributes(G4Colour::Blue());
  
     blue->SetVisibility(true);
     blue->SetForceAuxEdgeVisible(true);


     sphereVolume->SetVisAttributes(blue);


}  


G4VPhysicalVolume *DetectorConstruction::Construct()
{

  fBoxSize = 30*cm;


  sBox = new G4Box("world",                             //its name
                   fBoxSize/2,fBoxSize/2,fBoxSize/2);   //its dimensions

  fLBox = new G4LogicalVolume(sBox,                     //its shape
                             Vacc,                      //its material
                             "World");                  //its name

  fPBox = new G4PVPlacement(0,                          //no rotation
                            G4ThreeVector(),            //at (0,0,0)
                            fLBox,                      //its logical volume
                            "World",                    //its name
                            0,                          //its mother  volume
                            false,                      //no boolean operation
                            0);                         //copy number

  Sphereball(2.00*mm); 
 



  // shielding
/////...........first stack layer ...........................................................................

  fhThick = 0.06 * mm;   // this will be for a thickness of  0.12mm. halfZ

  if (fhThick == 0.06 * mm){
     G4double fhSize = 50 * mm;

     CreateAndPlaceShield(fhThick, fhSize, 32.00*mm, fLBox);
     ConstructOPPAC_1(33.56*mm);

     CreateAndPlaceShield(fhThick, fhSize, 35.12*mm, fLBox);
     ConstructOPPAC_2(36.68*mm);

     CreateAndPlaceShield(fhThick, fhSize, 38.24*mm, fLBox);
     ConstructOPPAC_3(39.8*mm);

     CreateAndPlaceShield(fhThick, fhSize, 41.36*mm, fLBox);
     ConstructOPPAC_4(42.92*mm);

     CreateAndPlaceShield(fhThick, fhSize, 44.48*mm, fLBox);
     ConstructOPPAC_5(46.04*mm);

     CreateAndPlaceShield(fhThick, fhSize, 47.6*mm, fLBox);
     ConstructOPPAC_6(49.16*mm);

     CreateAndPlaceShield(fhThick, fhSize, 50.72*mm, fLBox);
     ConstructOPPAC_7(52.28*mm);

     CreateAndPlaceShield(fhThick, fhSize, 53.84*mm, fLBox);
     ConstructOPPAC_8(55.4*mm);

     CreateAndPlaceShield(fhThick, fhSize, 56.96*mm, fLBox);
     ConstructOPPAC_9(58.52*mm);

     CreateAndPlaceShield(fhThick, fhSize, 60.08*mm, fLBox);
     ConstructOPPAC_10(61.64*mm);

  }



//................................end of first stack..........................................
//:::::::::::::::::::::::::::::::::::::second stack layer:::::::::::::::::::::::::::::::::::::
// 
   
   // this will be for a thickness of  0.10mm. halfZ

  else if (fhThick == 0.05 * mm){
          G4double fhSize = 50 * mm;

          CreateAndPlaceShield(fhThick, fhSize, 0.00*mm, fLBox);
          ConstructOPPAC_1(1.55*mm);

          CreateAndPlaceShield(fhThick, fhSize, 3.10*mm, fLBox);
          ConstructOPPAC_2(4.65*mm);

          CreateAndPlaceShield(fhThick, fhSize, 6.2*mm, fLBox);
          ConstructOPPAC_3(7.75*mm);

          CreateAndPlaceShield(fhThick, fhSize, 9.30*mm, fLBox);
          ConstructOPPAC_4(10.85*mm);

          CreateAndPlaceShield(fhThick, fhSize, 12.40*mm, fLBox);
          ConstructOPPAC_5(13.95*mm);

          CreateAndPlaceShield(fhThick, fhSize, 15.50*mm, fLBox);
          ConstructOPPAC_6(17.05*mm);

          CreateAndPlaceShield(fhThick, fhSize, 18.60*mm, fLBox);
          ConstructOPPAC_7(20.15*mm);

          CreateAndPlaceShield(fhThick, fhSize, 21.70*mm, fLBox);
          ConstructOPPAC_8(23.25*mm);

          CreateAndPlaceShield(fhThick, fhSize, 24.80*mm, fLBox);
          ConstructOPPAC_9(26.35*mm);

          CreateAndPlaceShield(fhThick, fhSize, 27.90*mm, fLBox);
          ConstructOPPAC_10(29.45*mm);

  }
//:::::::::::::::::::::::::::::::::::::end of second stack:::::::::::::::::::::::::::::::.:
  //:::::::::::::::::::::::::::::::::::::third stack layer:::::::::::::::::::::::::::::::::::::

  // this will be for a thickness of  0.14mm. halfZ

  else if (fhThick == 0.07 * mm){
          G4double fhSize = 50 * mm;

          CreateAndPlaceShield(fhThick, fhSize, 0.00*mm, fLBox);
          ConstructOPPAC_1(1.57*mm);

          CreateAndPlaceShield(fhThick, fhSize, 3.14*mm, fLBox);
          ConstructOPPAC_2(4.71*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 6.28*mm, fLBox);
  	  ConstructOPPAC_3(7.85*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 9.42*mm, fLBox);
  	  ConstructOPPAC_4(10.99*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 12.56*mm, fLBox);
  	  ConstructOPPAC_5(14.13*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 15.70*mm, fLBox);
  	  ConstructOPPAC_6(17.27*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 18.84*mm, fLBox);
  	  ConstructOPPAC_7(20.41*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 21.98*mm, fLBox);
  	  ConstructOPPAC_8(23.55*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 25.12*mm, fLBox);
  	  ConstructOPPAC_9(26.69*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 28.26*mm, fLBox);
  	  ConstructOPPAC_10(29.83*mm);
  }
//:::::::::::::::::::::::::::::::::::::end of third stack:::::::::::::::::::::::::::::::.:
   //:::::::::::::::::::::::::::::::::::::forth stack layer:::::::::::::::::::::::::::::::::::::

  // this will be for a thickness of  0.16mm. halfZ

  else if (fhThick == 0.08 * mm){
          G4double fhSize = 50 * mm;

  	  CreateAndPlaceShield(fhThick, fhSize, 0.00*mm, fLBox);
  	  ConstructOPPAC_1(1.58*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 3.16*mm, fLBox);
  	  ConstructOPPAC_2(4.74*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 6.32*mm, fLBox);
  	  ConstructOPPAC_3(7.9*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 9.48*mm, fLBox);
  	  ConstructOPPAC_4(11.06*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 12.64*mm, fLBox);
  	  ConstructOPPAC_5(14.22*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 15.80*mm, fLBox);
  	  ConstructOPPAC_6(17.38*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 18.96*mm, fLBox);
  	  ConstructOPPAC_7(20.54*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 22.12*mm, fLBox);
  	  ConstructOPPAC_8(23.70*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 25.28*mm, fLBox);
  	  ConstructOPPAC_9(26.86*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 28.44*mm, fLBox);
  	  ConstructOPPAC_10(30.02*mm);
  }
//:::::::::::::::::::::::::::::::::::::end of forth stack:::::::::::::::::::::::::::::::.:
//:::::::::::::::::::::::::::::::::::::fifth stack layer:::::::::::::::::::::::::::::::::::::

  // this will be for a thickness of  0.18mm. halfZ

  else if (fhThick == 0.09 * mm){
  	  G4double fhSize = 50 * mm;

  	  CreateAndPlaceShield(fhThick, fhSize, 0.00*mm, fLBox);
  	  ConstructOPPAC_1(1.59*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 3.18*mm, fLBox);
  	  ConstructOPPAC_2(4.77*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 6.36*mm, fLBox);
  	  ConstructOPPAC_3(7.95*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 9.54*mm, fLBox);
  	  ConstructOPPAC_4(11.13*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 12.72*mm, fLBox);
  	  ConstructOPPAC_5(14.31*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 15.90*mm, fLBox);
  	  ConstructOPPAC_6(17.49*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 19.08*mm, fLBox);
  	  ConstructOPPAC_7(20.67*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 22.26*mm, fLBox);
  	  ConstructOPPAC_8(23.85*mm);

          CreateAndPlaceShield(fhThick, fhSize, 25.44*mm, fLBox);
  	  ConstructOPPAC_9(27.03*mm);

          CreateAndPlaceShield(fhThick, fhSize, 28.62*mm, fLBox);
          ConstructOPPAC_10(30.21*mm);
  }
//:::::::::::::::::::::::::::::::::::::end of fifth stack:::::::::::::::::::::::::::::::.:

//:::::::::::::::::::::::::::::::::::::sixth stack layer:::::::::::::::::::::::::::::::::::::

  // this will be for a thickness of  0.20mm. halfZ
  else if (fhThick == 0.10 * mm){
  	  G4double fhSize = 50 * mm;

  	  CreateAndPlaceShield(fhThick, fhSize, 0.00*mm, fLBox);
  	  ConstructOPPAC_1(1.60*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 3.20*mm, fLBox);
  	  ConstructOPPAC_2(4.8*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 6.40*mm, fLBox);
  	  ConstructOPPAC_3(8.0*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 9.60*mm, fLBox);
  	  ConstructOPPAC_4(11.20*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 12.80*mm, fLBox);
  	  ConstructOPPAC_5(14.40*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 16.00*mm, fLBox);
  	  ConstructOPPAC_6(17.60*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 19.20*mm, fLBox);
  	  ConstructOPPAC_7(20.80*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 22.40*mm, fLBox);
  	  ConstructOPPAC_8(24.0*mm);

          CreateAndPlaceShield(fhThick, fhSize, 25.60*mm, fLBox);
          ConstructOPPAC_9(27.20*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 28.80*mm, fLBox);
  	  ConstructOPPAC_10(30.40*mm);
  }
//:::::::::::::::::::::::::::::::::::::end of sixth stack:::::::::::::::::::::::::::::::.:

 // this will be for a thickness of  0.22mm. halfZ

 else if (fhThick == 0.11 * mm){
          G4double fhSize = 50 * mm;

          CreateAndPlaceShield(fhThick, fhSize, 0.00*mm, fLBox);
          ConstructOPPAC_1(1.61*mm);

          CreateAndPlaceShield(fhThick, fhSize, 3.22*mm, fLBox);
          ConstructOPPAC_2(4.83*mm);

          CreateAndPlaceShield(fhThick, fhSize, 6.44*mm, fLBox);
          ConstructOPPAC_3(8.05*mm);

          CreateAndPlaceShield(fhThick, fhSize, 9.66*mm, fLBox);
          ConstructOPPAC_4(11.27*mm);

          CreateAndPlaceShield(fhThick, fhSize, 12.88*mm, fLBox);
          ConstructOPPAC_5(14.49*mm);

          CreateAndPlaceShield(fhThick, fhSize, 16.10*mm, fLBox);
          ConstructOPPAC_6(17.71*mm);

          CreateAndPlaceShield(fhThick, fhSize, 19.32*mm, fLBox);
          ConstructOPPAC_7(20.93*mm);

          CreateAndPlaceShield(fhThick, fhSize, 22.54*mm, fLBox);
          ConstructOPPAC_8(24.15*mm);

          CreateAndPlaceShield(fhThick, fhSize, 25.76*mm, fLBox);
          ConstructOPPAC_9(27.37*mm);

          CreateAndPlaceShield(fhThick, fhSize, 28.98*mm, fLBox);
          ConstructOPPAC_10(30.59*mm);
  }


//:::::::::::::::::::::::::::::::::::::seventh stack layer:::::::::::::::::::::::::::::::::::::

  // this will be for a thickness of  0.24mm. halfZ

  else if (fhThick == 0.12 * mm){
  	  G4double fhSize = 50 * mm;

  	  CreateAndPlaceShield(fhThick, fhSize, 0.00*mm, fLBox);
  	  ConstructOPPAC_1(1.62*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 3.24*mm, fLBox);
  	  ConstructOPPAC_2(4.86*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 6.48*mm, fLBox);
  	  ConstructOPPAC_3(8.1*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 9.72*mm, fLBox);
  	  ConstructOPPAC_4(11.34*mm);

 	  CreateAndPlaceShield(fhThick, fhSize, 12.96*mm, fLBox);
  	  ConstructOPPAC_5(14.58*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 16.20*mm, fLBox);
  	  ConstructOPPAC_6(17.82*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 19.44*mm, fLBox);
  	  ConstructOPPAC_7(21.06*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 22.68*mm, fLBox);
          ConstructOPPAC_8(24.3*mm);

          CreateAndPlaceShield(fhThick, fhSize, 25.92*mm, fLBox);
          ConstructOPPAC_9(27.54*mm);

          CreateAndPlaceShield(fhThick, fhSize, 29.16*mm, fLBox);
          ConstructOPPAC_10(30.78*mm);
  }
//:::::::::::::::::::::::::::::::::::::end of seventh stack:::::::::::::::::::::::::::::::.:

//:::::::::::::::::::::::::::::::::::::Eigth stack layer:::::::::::::::::::::::::::::::::::::

 // this will be for a thickness of  0.26mm. halfZ

  else if (fhThick == 0.13 * mm){
          G4double fhSize = 50 * mm;

  	  CreateAndPlaceShield(fhThick, fhSize, 0.00*mm, fLBox);
  	  ConstructOPPAC_1(1.63*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 3.26*mm, fLBox);
  	  ConstructOPPAC_2(4.89*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 6.52*mm, fLBox);
  	  ConstructOPPAC_3(8.15*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 9.78*mm, fLBox);
  	  ConstructOPPAC_4(11.41*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 13.04*mm, fLBox);
  	  ConstructOPPAC_5(14.67*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 16.30*mm, fLBox);
          ConstructOPPAC_6(17.93*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 19.56*mm, fLBox);
  	  ConstructOPPAC_7(21.19*mm);

 	  CreateAndPlaceShield(fhThick, fhSize, 22.82*mm, fLBox);
  	  ConstructOPPAC_8(24.45*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 26.08*mm, fLBox);
  	  ConstructOPPAC_9(27.71*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 29.34*mm, fLBox);
  	  ConstructOPPAC_10(30.97*mm);
  }
//:::::::::::::::::::::::::::::::::::::end of eight stack:::::::::::::::::::::::::::::::.:


//:::::::::::::::::::::::::::::::::::::ninth stack layer:::::::::::::::::::::::::::::::::::::

  // this will be for a thickness of  0.28mm. halfZ

  else if (fhThick == 0.14 * mm){
  	  G4double fhSize = 50 * mm;

  	  CreateAndPlaceShield(fhThick, fhSize, 0.00*mm, fLBox);
  	  ConstructOPPAC_1(1.64*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 3.28*mm, fLBox);
  	  ConstructOPPAC_2(4.92*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 6.56*mm, fLBox);
          ConstructOPPAC_3(8.2*mm);

 	  CreateAndPlaceShield(fhThick, fhSize, 9.84*mm, fLBox);
  	  ConstructOPPAC_4(11.48*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 13.12*mm, fLBox);
  	  ConstructOPPAC_5(14.76*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 16.40*mm, fLBox);
  	  ConstructOPPAC_6(18.04*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 19.68*mm, fLBox);
  	  ConstructOPPAC_7(21.32*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 22.96*mm, fLBox);
  	  ConstructOPPAC_8(24.60*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 26.24*mm, fLBox);
  	  ConstructOPPAC_9(27.88*mm);

  	  CreateAndPlaceShield(fhThick, fhSize, 29.52*mm, fLBox);
  	  ConstructOPPAC_10(31.16*mm);
  }
//:::::::::::::::::::::::::::::::::::::end of ninth stack:::::::::::::::::::::::::::::::.:

//:::::::::::::::::::::::::::::::::::: tenth stack layer:::::::::::::::::::::::::::::::::::::

  // this will be for a thickness of  0.30mm. halfZ

  else if  (fhThick == 0.15 * mm){
           G4double fhSize = 50 * mm;

           CreateAndPlaceShield(fhThick, fhSize, 0.00*mm, fLBox);
           ConstructOPPAC_1(1.65*mm);

  	   CreateAndPlaceShield(fhThick, fhSize, 3.30*mm, fLBox);
  	   ConstructOPPAC_2(4.95*mm);

  	   CreateAndPlaceShield(fhThick, fhSize, 6.6*mm, fLBox);
  	   ConstructOPPAC_3(8.25*mm);

 	   CreateAndPlaceShield(fhThick, fhSize, 9.9*mm, fLBox);
  	   ConstructOPPAC_4(11.55*mm);

  	   CreateAndPlaceShield(fhThick, fhSize, 13.20*mm, fLBox);
  	   ConstructOPPAC_5(14.85*mm);

  	   CreateAndPlaceShield(fhThick, fhSize, 16.50*mm, fLBox);
  	   ConstructOPPAC_6(18.15*mm);

  	   CreateAndPlaceShield(fhThick, fhSize, 19.80*mm, fLBox);
  	   ConstructOPPAC_7(21.45*mm);

  	   CreateAndPlaceShield(fhThick, fhSize, 23.10*mm, fLBox);
  	   ConstructOPPAC_8(24.75*mm);

  	   CreateAndPlaceShield(fhThick, fhSize, 26.40*mm, fLBox);
  	   ConstructOPPAC_9(28.05*mm);

  	   CreateAndPlaceShield(fhThick, fhSize, 29.70*mm, fLBox);
  	   ConstructOPPAC_10(31.35*mm);
  }
//:::::::::::::::::::::::::::::::::::::end of tenth stack:::::::::::::::::::::::::::::::.:


  return fPBox;
}

//...ooooooooooooooooo..................................oooooooooooooooooooo......



void DetectorConstruction::ConstructSDandField()
{
 // sensitive detectors -----------------------------------------------------
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->SetVerboseLevel(0);

  //Define Multi-Detector and Register 
  //--------------------------------------------------------------------------------------------
  G4MultiFunctionalDetector* det = new G4MultiFunctionalDetector("IonPro");
  SDman->AddNewDetector(det);
//  fLScore->SetSensitiveDetector(det);
}
