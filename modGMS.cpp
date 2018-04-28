#include <Rcpp.h>
#include <math.h>
using namespace Rcpp;

// [[Rcpp::export]]
List modGMSrcpp(double t, NumericVector state, NumericVector parameters) 
{
  // switches for interventions
  double EDATon = parameters["EDATon"];
  double ITNon = parameters["ITNon"];
  double IRSon = parameters["IRSon"];
  double MDAon = parameters["MDAon"];
  double primon = parameters["primon"];
  double MSATon = parameters["MSATon"];
  double VACon = parameters["VACon"];
  
  // convert %s to proportions
  double covEDATi=parameters["covEDATi"];
    covEDATi = 0.9*covEDATi/100;
  double covEDAT0=parameters["covEDAT0"];
    covEDAT0 = 0.9*covEDAT0/100;
  double covITNi=parameters["covITNi"];
    covITNi = covITNi/100;
  double covITN0=parameters["covITN0"];
    covITN0 = covITN0/100;
  double effITN = parameters["effITN"];
    effITN =effITN/100;
  double covIRSi=parameters["covIRSi"];
    covIRSi=covIRSi/100;
  double covIRS0=parameters["covIRS0"];
    covIRS0=covIRS0/100;
  double effIRS = parameters["effIRS"];
    effIRS=effIRS/100;
  double covMSATi=parameters["covMSATi"];
    covMSATi=covMSATi/100;
  double covMSAT0=parameters["covMSAT0"];
    covMSAT0=covMSAT0/100;
  double MSATsensC=parameters["MSATsensC"];
    MSATsensC=MSATsensC/100;
  double MSATsensA=parameters["MSATsensA"];
    MSATsensA=MSATsensA/100;
  double MSATsensU=parameters["MSATsensU"];
    MSATsensU=MSATsensU/100;
  double cm_1=parameters["cm_1"];
    cm_1=cm_1/100;
  double cm_2=parameters["cm_2"];
    cm_2=cm_2/100;
  double cm_3=parameters["cm_3"];
    cm_3=cm_3/100;
  double cmda_1=parameters["cmda_1"];
    cmda_1=cmda_1/100;
  double cmda_2=parameters["cmda_2"];
    cmda_2=cmda_2/100;
  double cmda_3=parameters["cmda_3"];
    cmda_3=cmda_3/100;
  double cmda_4=0; // booster vaccine, coverage for MdA is 0
  double effv_1=parameters["effv_1"];
    effv_1=effv_1/100;
  double effv_2=parameters["effv_2"];
    effv_2=effv_2/100;
  double effv_3=parameters["effv_3"];
    effv_3=effv_3/100;
  double effv_4= effv_3;  // booster vaccine, effect of vaccine is assumed to resemble the 3rd dose
  double rhoa=parameters["rhoa"];
    rhoa=rhoa/100;
  double rhou=parameters["rhou"];
    rhou=rhou/100;
  double ps=parameters["ps"];
    ps=ps/100;
  double pr=parameters["pr"];
    pr=pr/100;
  double eta=parameters["eta"];
    eta=eta/100;
  
  // convert time scales
  double dm=parameters["dm"];
    dm=dm/12;
  double tm_1 = parameters["tm_1"];
    tm_1=2018+(tm_1/12);
  double tm_2 = parameters["tm_2"];
    tm_2=2018+(tm_2/12);
  double tm_3 = parameters["tm_3"];
    tm_3=2018+(tm_3/12);
  double tm_4=tm_1+1; // booster vaccine
  
  
  // convert durations to rates
  double lossd=parameters["lossd"];
    lossd=365/lossd;
  double omega=parameters["omega"];
    omega=1/omega;
  double nuC=parameters["nuC"];
    nuC=365/nuC;
  double nuA=parameters["nuA"];
    nuA=365/nuA;
  double nuU=parameters["nuU"];
    nuU=365/nuU;
  double mu=parameters["mu"];
    mu=1/mu;
  double nTr=parameters["nuTr"];
    nTr=365/nTr;
  double nTrp=parameters["nuTrp"];
    nTrp=365/nTrp;
  double muC=parameters["muC"];
    muC=muC/1000;
  double muA=parameters["muA"];
    muA=muA/1000;
  double muU=parameters["muU"];
    muU=muU/1000;
  
  //remaining variables
  double vh = parameters["vh"];
    vh=vh/365;
  double timei = parameters["timei"];
  double alpha = parameters["alpha"];
  double phi =  parameters["phi"];
  double epsilonh = parameters["epsilonh"];
  double epsilonm = parameters["epsilonm"];
  double b = parameters["b"];
  double deltam=parameters["deltam"];                 
  double gammam=parameters["gammam"];
  double percfail2018 = parameters["percfail2018"];
  double percfail2019 = parameters["percfail2019"];
  double percfail2020 = parameters["percfail2020"];
  double EDATscale =parameters["EDATscale"];
  double ITNscale=parameters["ITNscale"] ;
  double IRSscale =parameters["IRSscale"];
  double MSATscale=parameters["MSATscale"];
  double bh_max =parameters["bh_max"];
  double startyear=2007;
  
  // states
  double Y = state["Y"];
  double S_0 = state["S_0"];
  double IC_0 = state["IC_0"];
  double IA_0 = state["IA_0"];
  double IU_0 = state["IU_0"];
  double R_0 = state["R_0"];
  double Tr_0 = state["Tr_0"];
  double Sm_0 = state["Sm_0"];
  double Rm_0 = state["Rm_0"];
  double S_1 = state["S_1"];
  double IC_1 = state["IC_1"];
  double IA_1 = state["IA_1"];
  double IU_1 = state["IU_1"];
  double R_1 = state["R_1"];
  double Tr_1 = state["Tr_1"];
  double Sm_1 = state["Sm_1"];
  double Rm_1 = state["Rm_1"];
  double S_2 = state["S_2"];
  double IC_2 = state["IC_2"];
  double IA_2 = state["IA_2"];
  double IU_2 = state["IU_2"];
  double R_2 = state["R_2"];
  double Tr_2 = state["Tr_2"];
  double Sm_2 = state["Sm_2"];
  double Rm_2 = state["Rm_2"];
  double S_3 = state["S_3"];
  double IC_3 = state["IC_3"];
  double IA_3 = state["IA_3"];
  double IU_3 = state["IU_3"];
  double R_3 = state["R_3"];
  double Tr_3 = state["Tr_3"];
  double Sm_3 = state["Sm_3"];
  double Rm_3 = state["Rm_3"];
  double S_4 = state["S_4"];
  double IC_4 = state["IC_4"];
  double IA_4 = state["IA_4"];
  double IU_4 = state["IU_4"];
  double R_4 = state["R_4"];
  double Tr_4 = state["Tr_4"];
  double Sm_4 = state["Sm_4"];
  double Rm_4 = state["Rm_4"];
  
  // swtich on double interventions
  covEDATi = EDATon*covEDATi+(1-EDATon)*covEDAT0;
  covITNi = ITNon*covITNi+(1-ITNon)*covITN0;
  covIRSi = IRSon*covIRSi+(1-IRSon)*covIRS0;
  
  double sS = S_0+S_1+S_2+S_3+S_4;
  double sR = R_0+R_1+R_2+R_3+R_4;
  double sIC = IC_0+IC_1+IC_2+IC_3+IC_4;
  double sIA = IA_0+IA_1+IA_2+IA_3+IA_4;
  double sIU = IU_0+IU_1+IU_2+IU_3+IU_4;
  double sTr = Tr_0+Tr_1+Tr_2+Tr_3+Tr_4;
  double sSm = Sm_0+Sm_1+Sm_2+Sm_3+Sm_4;
  double sRm = Rm_0+Rm_1+Rm_2+Rm_3+Rm_4;
  
  // define variables
  double P = (sS+sR+sIC+sIA+sIU+sTr+sSm+sRm);
  double seas=1+alpha*cos(2*3.14159*(Y-phi));
  double bh=bh_max/(1+alpha);
  
  // Additional file: Equation no.10
  double beta=seas*b*epsilonh*epsilonm*bh/((bh*epsilonh+deltam)*(gammam/(gammam+deltam)));
  
  double mu_out = mu+muC+muA+muU;
  
  timei=timei-startyear;
  
  // Additional file: Equation no.14
  double wsiEDAT=(1-(Y<=timei))*(Y<=(timei+EDATscale))*((Y-timei)/EDATscale)+1*(Y>(timei+EDATscale));
  double wsiITN=(1-(Y<=timei))*(Y<=(timei+ITNscale))*((Y-timei)/ITNscale)+1*(Y>(timei+ITNscale));
  double wsiIRS=(1-(Y<=timei))*(Y<=(timei+IRSscale))*((Y-timei)/IRSscale)+1*(Y>(timei+IRSscale));
  double wsiMSAT=(1-(Y<=timei))*(Y<=(timei+MSATscale))*((Y-timei)/MSATscale)+1*(Y>(timei+MSATscale));
  double covEDAT=(1-wsiEDAT)*covEDAT0+wsiEDAT*covEDATi;
  double covITN=(1-wsiITN)*covITN0+wsiITN*covITNi;
  double covIRS=(1-wsiIRS)*covIRS0+wsiIRS*covIRSi;
  double covMSAT=(1-wsiMSAT)*covMSAT0+wsiMSAT*covMSATi;
  
  double nuTr= primon*((Y<timei)*nTr+(Y>timei)*nTrp)+(1-primon)*nTr;
  lossd=1/((1/lossd)-(1/nuTr));
  
  // Additional file: Equation no.9
  double lam = (1-(1-eta)*effIRS*covIRS)*(1-effITN*covITN)*beta*(sIC+sTr+rhoa*sIA+rhou*sIU)/P;
  
  // vaccine effects
  // Additional file: Equation no.17
  double v_1 = VACon*((Y>(tm_1-startyear))*(Y<=tm_1-startyear+dm)*(std::min(effv_1*((vh/(dm*log(2)))+(1-(vh/(dm*log(2))))*exp(-(Y+startyear-tm_1)*(log(2)/vh))), effv_1))+(Y>tm_1-startyear+dm)*(std::min(effv_1*((vh/(dm*log(2)))+(1-(vh/(dm*log(2))))*exp(-log(2)*dm/vh))*exp(-(Y+startyear-tm_1-dm)*(log(2)/vh)),effv_1)));
  double v_2 = VACon*((Y>(tm_2-startyear))*(Y<=tm_2-startyear+dm)*(std::min(effv_2*((vh/(dm*log(2)))+(1-(vh/(dm*log(2))))*exp(-(Y+startyear-tm_2)*(log(2)/vh))), effv_2))+(Y>tm_2-startyear+dm)*(std::min(effv_2*((vh/(dm*log(2)))+(1-(vh/(dm*log(2))))*exp(-log(2)*dm/vh))*exp(-(Y+startyear-tm_2-dm)*(log(2)/vh)),effv_2)));
  double v_3 = VACon*((Y>(tm_3-startyear))*(Y<=tm_3-startyear+dm)*(std::min(effv_3*((vh/(dm*log(2)))+(1-(vh/(dm*log(2))))*exp(-(Y+startyear-tm_3)*(log(2)/vh))), effv_3))+(Y>tm_3-startyear+dm)*(std::min(effv_3*((vh/(dm*log(2)))+(1-(vh/(dm*log(2))))*exp(-log(2)*dm/vh))*exp(-(Y+startyear-tm_3-dm)*(log(2)/vh)),effv_3)));
  double v_4 = VACon*((Y>(tm_4-startyear))*(Y<=tm_4-startyear+dm)*(std::min(effv_4*((vh/(dm*log(2)))+(1-(vh/(dm*log(2))))*exp(-(Y+startyear-tm_4)*(log(2)/vh))), effv_4))+(Y>tm_4-startyear+dm)*(std::min(effv_4*((vh/(dm*log(2)))+(1-(vh/(dm*log(2))))*exp(-log(2)*dm/vh))*exp(-(Y+startyear-tm_4-dm)*(log(2)/vh)),effv_4)));
  
  // Additional file: Equation no.16
  double lam_1 = (1-v_1)*lam;
  double lam_2 = (1-v_2)*lam;
  double lam_3 = (1-v_3)*lam;
  double lam_4 = (1-v_4)*lam;
  //double lam_1 = (1-(cmda_1*v_1))*lam;
  //double lam_2 = (1-(cmda_2*v_2))*lam;
  //double lam_3 = (1-(cmda_3*v_3))*lam;
  //double lam_4 = (1-(cmda_4*v_4))*lam;
  
  double tau = covEDAT;
  
  //double fail = ((Y+startyear)<2019)*(percfail2018/100)+((Y+startyear)>=2019)*((Y+startyear)<2020)*(percfail2019/100)+((Y+startyear)>=2020)*(percfail2020/100);
  double fail = ((Y+startyear)<2018)*(percfail2018/100)+((Y+startyear)>=2018)*((Y+startyear)<2019)*(((percfail2019-percfail2018)/100)*(Y+startyear-2019)+(percfail2018/100))+((Y+startyear)>=2019)*((Y+startyear)<2020)*(((percfail2020-percfail2019)/100)*(Y+startyear-2020)+(percfail2019/100))+((Y+startyear)>=2020)*(percfail2020/100);

  // MDA and RTS,S rounds
  // Additional file: Equation no.15
  double m_1= MDAon*(Y>(tm_1-startyear))*(Y<=(tm_1+dm-startyear))*(-log((1-cm_1))/dm);
  double m_2= MDAon*(Y>(tm_2-startyear))*(Y<=(tm_2+dm-startyear))*(-log((1-cm_2))/dm);
  double m_3= MDAon*(Y>(tm_3-startyear))*(Y<=(tm_3+dm-startyear))*(-log((1-cm_3))/dm); 
  double m_4= MDAon*(Y>(tm_4-startyear))*(Y<=(tm_4+dm-startyear))*(-log((1-cm_3))/dm); // coverage of m4 is the same as m3
  double m_5= 0;
  
  // Additional file: Equation no.18 
  muC = (1-MSATon*MSATsensC*covMSAT)*muC;
  muA = (1-MSATon*MSATsensA*covMSAT)*muA;
  muU = (1-MSATon*MSATsensU*covMSAT)*muU;
  
  
  // rate of change
  // Additional file: Equation no. 1 - 8
  double dY = 1;
  
  double dCinc_det = ps*tau*lam*sS+pr*tau*lam*sR+pr*tau*lam*sIU+pr*tau*lam*sIA;                           //3 // Additional file: Equation no.12
  double dCinc_tot = ps*lam*sS+pr*lam*sR+pr*lam*sIU+pr*lam*sIA;                                                                                                 //4 // Additional file: Equation no.11
  double dS_0 = mu*P-mu_out*S_0+omega*R_0-lam*S_0+lossd*Sm_0-m_1*S_0;                                                                                         //5
  double dIC_0 = muC*P-mu_out*IC_0+ps*(1-tau)*lam*S_0+pr*(1-tau)*lam*R_0+pr*(1-tau)*lam*IU_0+pr*(1-tau)*lam*IA_0-nuC*IC_0-m_1*IC_0;        //6 
  double dIA_0 = muA*P-mu_out*IA_0+(1-ps)*lam*S_0+(1-pr)*lam*R_0+(1-pr)*lam*IU_0-pr*lam*IA_0+nuC*IC_0-nuA*IA_0+fail*nuTr*Tr_0-m_1*IA_0;    //7
  double dIU_0 = muU*P-mu_out*IU_0-lam*IU_0-nuU*IU_0+nuA*IA_0-m_1*IU_0;                                                                    //8
  double dR_0 = -mu_out*R_0-omega*R_0-lam*R_0+nuU*IU_0+lossd*Rm_0-m_1*R_0;                                                                                      //9
  double dTr_0 = -mu_out*Tr_0+ps*tau*lam*S_0+pr*tau*lam*R_0+pr*tau*lam*IU_0+pr*tau*lam*IA_0-nuTr*Tr_0-m_1*Tr_0; //10
  double dSm_0 = -mu_out*Sm_0+omega*Rm_0-lossd*Sm_0-m_1*Sm_0;                                                                                                   //11
  double dRm_0 = -mu_out*Rm_0-omega*Rm_0+(1-fail)*nuTr*Tr_0-lossd*Rm_0-m_1*Rm_0;                                                                                //12
  
  
  double dS_1 = -mu_out*S_1+omega*R_1-lam_1*S_1+lossd*Sm_1+(1-cmda_1)*m_1*S_0-m_2*S_1;                                                                          //13
  double dIC_1 = -mu_out*IC_1+ps*(1-tau)*lam_1*S_1+pr*(1-tau)*lam_1*R_1+pr*(1-tau)*lam_1*IU_1+pr*(1-tau)*lam_1*IA_1-nuC*IC_1+(1-cmda_1)*m_1*IC_0-m_2*IC_1;      //14
  double dIA_1 = -mu_out*IA_1+(1-ps)*lam_1*S_1+(1-pr)*lam_1*R_1+(1-pr)*lam_1*IU_1-pr*lam_1*IA_1+nuC*IC_1-nuA*IA_1+fail*nuTr*Tr_1+(1-cmda_1)*m_1*IA_0-m_2*IA_1;  //15
  double dIU_1 = -mu_out*IU_1-lam_1*IU_1-nuU*IU_1+nuA*IA_1+(1-cmda_1)*m_1*IU_0-m_2*IU_1;                                                                        //16
  double dR_1 = -mu_out*R_1-omega*R_1-lam_1*R_1+nuU*IU_1 +lossd*Rm_1+(1-cmda_1)*m_1*R_0-m_2*R_1;                                                                //17
  double dTr_1 = -mu_out*Tr_1+ps*tau*lam_1*S_1+pr*tau*lam_1*R_1+pr*tau*lam_1*IU_1+pr*tau*lam_1*IA_1-nuTr*Tr_1+m_1*(cmda_1*(IC_0+IA_0+IU_0)+Tr_0)-m_2*Tr_1;      //18
  double dSm_1 = -mu_out*Sm_1+omega*Rm_1-lossd*Sm_1+m_1*(cmda_1*S_0+Sm_0)-m_2*Sm_1;                                                                             //19
  double dRm_1 = -mu_out*Rm_1-omega*Rm_1+(1-fail)*nuTr*Tr_1-lossd*Rm_1+m_1*(cmda_1*R_0+Rm_0)-m_2*Rm_1;                                                          //20
  
  double dS_2 = -mu_out*S_2+omega*R_2-lam_2*S_2+lossd*Sm_2+(1-cmda_2)*m_2*S_1-m_3*S_2;                                                                          //21
  double dIC_2 = -mu_out*IC_2+ps*(1-tau)*lam_2*S_2+pr*(1-tau)*lam_2*R_2+pr*(1-tau)*lam_2*IU_2+pr*(1-tau)*lam_2*IA_2-nuC*IC_2+(1-cmda_2)*m_2*IC_1-m_3*IC_2;      //22
  double dIA_2 = -mu_out*IA_2+(1-ps)*lam_2*S_2+(1-pr)*lam_2*R_2+(1-pr)*lam_2*IU_2-pr*lam_2*IA_2+nuC*IC_2-nuA*IA_2+fail*nuTr*Tr_2+(1-cmda_2)*m_2*IA_1-m_3*IA_2;  //23
  double dIU_2 = -mu_out*IU_2-lam_2*IU_2-nuU*IU_2+nuA*IA_2+(1-cmda_2)*m_2*IU_1-m_3*IU_2;                                                                        //24
  double dR_2 = -mu_out*R_2-omega*R_2-lam_2*R_2+nuU*IU_2 +lossd*Rm_2+(1-cmda_2)*m_2*R_1-m_3*R_2;                                                                //25
  double dTr_2 = -mu_out*Tr_2+ps*tau*lam_2*S_2+pr*tau*lam_2*R_2+pr*tau*lam_2*IU_2+pr*tau*lam_2*IA_2-nuTr*Tr_2+m_2*(cmda_2*(IC_1+IA_1+IU_1)+Tr_1)-m_3*Tr_2;      //26
  double dSm_2 = -mu_out*Sm_2+omega*Rm_2-lossd*Sm_2+m_2*(cmda_2*S_1+Sm_1)-m_3*Sm_2;                                                                             //27
  double dRm_2 = -mu_out*Rm_2-omega*Rm_2+(1-fail)*nuTr*Tr_2-lossd*Rm_2+m_2*(cmda_2*R_1+Rm_1)-m_3*Rm_2;                                                          //28
  
  double dS_3 = -mu_out*S_3+omega*R_3-lam_3*S_3+lossd*Sm_3+(1-cmda_3)*m_3*S_2-m_4*S_3;                                                                          //29
  double dIC_3 = -mu_out*IC_3+ps*(1-tau)*lam_3*S_3+pr*(1-tau)*lam_3*R_3+pr*(1-tau)*lam_3*IU_3+pr*(1-tau)*lam_3*IA_3-nuC*IC_3+(1-cmda_3)*m_3*IC_2-m_4*IC_3;      //30
  double dIA_3 = -mu_out*IA_3+(1-ps)*lam_3*S_3+(1-pr)*lam_3*R_3+(1-pr)*lam_3*IU_3-pr*lam_3*IA_3+nuC*IC_3-nuA*IA_3+fail*nuTr*Tr_3+(1-cmda_3)*m_3*IA_2-m_4*IA_3;  //31
  double dIU_3 = -mu_out*IU_3-lam_3*IU_3-nuU*IU_3+nuA*IA_3+(1-cmda_3)*m_3*IU_2-m_4*IU_3;                                                                        //32
  double dR_3 = -mu_out*R_3-omega*R_3-lam_3*R_3+nuU*IU_3 +lossd*Rm_3+(1-cmda_3)*m_3*R_2-m_4*R_3;                                                                //33
  double dTr_3 = -mu_out*Tr_3+ps*tau*lam_3*S_3+pr*tau*lam_3*R_3+pr*tau*lam_3*IU_3+pr*tau*lam_3*IA_3-nuTr*Tr_3+m_3*(cmda_3*(IC_2+IA_2+IU_2)+Tr_2)-m_4*Tr_3;      //34
  double dSm_3 = -mu_out*Sm_3+omega*Rm_3-lossd*Sm_3+m_3*(cmda_3*S_2+Sm_2)-m_4*Sm_3;                                                                             //35
  double dRm_3 = -mu_out*Rm_3-omega*Rm_3+(1-fail)*nuTr*Tr_3-lossd*Rm_3+m_3*(cmda_3*R_2+Rm_2)-m_4*Rm_3;                                                          //36
  
  double dS_4 = -mu_out*S_4+omega*R_4-lam_4*S_4+lossd*Sm_4+(1-cmda_4)*m_4*S_3-m_5*S_4;                                                                          //37
  double dIC_4 = -mu_out*IC_4+ps*(1-tau)*lam_4*S_4+pr*(1-tau)*lam_4*R_4+pr*(1-tau)*lam_4*IU_4+pr*(1-tau)*lam_4*IA_4-nuC*IC_4+(1-cmda_4)*m_4*IC_3-m_5*IC_4;      //38
  double dIA_4 = -mu_out*IA_4+(1-ps)*lam_4*S_4+(1-pr)*lam_4*R_4+(1-pr)*lam_4*IU_4-pr*lam_4*IA_4+nuC*IC_4-nuA*IA_4+fail*nuTr*Tr_4+(1-cmda_4)*m_4*IA_3-m_5*IA_4;  //39
  double dIU_4 = -mu_out*IU_4-lam_4*IU_4-nuU*IU_4+nuA*IA_4+(1-cmda_4)*m_4*IU_3-m_5*IU_4;                                                                        //40
  double dR_4 = -mu_out*R_4-omega*R_4-lam_4*R_4+nuU*IU_4 +lossd*Rm_4+(1-cmda_4)*m_4*R_3-m_5*R_4;                                                                //41
  double dTr_4 = -mu_out*Tr_4+ps*tau*lam_4*S_4+pr*tau*lam_4*R_4+pr*tau*lam_4*IU_4+pr*tau*lam_4*IA_4-nuTr*Tr_4+m_4*(cmda_4*(IC_3+IA_3+IU_3)+Tr_3)-m_5*Tr_4;      //42
  double dSm_4 = -mu_out*Sm_4+omega*Rm_4-lossd*Sm_4+m_4*(cmda_4*S_3+Sm_3)-m_5*Sm_4;                                                                             //43
  double dRm_4 = -mu_out*Rm_4-omega*Rm_4+(1-fail)*nuTr*Tr_4-lossd*Rm_4+m_4*(cmda_4*R_3+Rm_3)-m_5*Rm_4;                                                          //44
  
  // return the rate of change
  List output;
  output["dY"]=dY;
  output["dCinc_det"]=dCinc_det;
  output["dCinc_tot"]=dCinc_tot;
  output["dS_0"]=dS_0;
  output["dIC_0"]=dIC_0;
  output["dIA_0"]=dIA_0;
  output["dIU_0"]=dIU_0;
  output["dR_0"]=dR_0;
  output["dTr_0"]=dTr_0;
  output["dSm_0"]=dSm_0;
  output["dRm_0"]=dRm_0;
  output["dS_1"]=dS_1;
  output["dIC_1"]=dIC_1;
  output["dIA_1"]=dIA_1;
  output["dIU_1"]=dIU_1;
  output["dR_1"]=dR_1;
  output["dTr_1"]=dTr_1;
  output["dSm_1"]=dSm_1;
  output["dRm_1"]=dRm_1;
  output["dS_2"]=dS_2;
  output["dIC_2"]=dIC_2;
  output["dIA_2"]=dIA_2;
  output["dIU_2"]=dIU_2;
  output["dR_2"]=dR_2;
  output["dTr_2"]=dTr_2;
  output["dSm_2"]=dSm_2;
  output["dRm_2"]=dRm_2;
  output["dS_3"]=dS_3;
  output["dIC_3"]=dIC_3;
  output["dIA_3"]=dIA_3;
  output["dIU_3"]=dIU_3;
  output["dR_3"]=dR_3;
  output["dTr_3"]=dTr_3;
  output["dSm_3"]=dSm_3;
  output["dRm_3"]=dRm_3;
  output["dS_4"]=dS_4;
  output["dIC_4"]=dIC_4;
  output["dIA_4"]=dIA_4;
  output["dIU_4"]=dIU_4;
  output["dR_4"]=dR_4;
  output["dTr_4"]=dTr_4;
  output["dSm_4"]=dSm_4;
  output["dRm_4"]=dRm_4;

  return output;
    
}

