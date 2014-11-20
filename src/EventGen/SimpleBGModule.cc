#include <TH1F.h>
#include "SimpleBGModule.hh"
#include "DetectorConstruction.hh"

#include <G4Neutron.hh>
#include <G4Gamma.hh>
#include <G4ParticleGun.hh>
#include <G4ios.hh>
#include <G4UnitsTable.hh>
#include <G4SystemOfUnits.hh>
#include <G4VSolid.hh>

#include <cassert>
#include <cmath>


SimpleBGModule::SimpleBGModule(PrimaryGeneratorAction* P):
PrimaryGeneratorModule(P, "SimpleBG"),
netRate(1/s) {
    const unsigned int nBins = 400;

    float ebins[nBins] = {0.01, 0.03, 0.05, 0.07, 0.09, 0.11, 0.13, 0.15, 0.17, 0.19, 0.21, 0.23, 0.25, 0.27, 0.29, 0.31, 0.33, 0.35, 0.37, 0.39, 0.41, 0.43, 0.45, 0.47, 0.49, 0.51, 0.53, 0.55, 0.57, 0.59, 0.61, 0.63, 0.65, 0.67, 0.69, 0.71, 0.73, 0.75, 0.77, 0.79, 0.81, 0.83, 0.85, 0.87, 0.89, 0.91, 0.93, 0.95, 0.97, 0.99, 1.01, 1.03, 1.05, 1.07, 1.09, 1.11, 1.13, 1.15, 1.17, 1.19, 1.21, 1.23, 1.25, 1.27, 1.29, 1.31, 1.33, 1.35, 1.37, 1.39, 1.41, 1.43, 1.45, 1.47, 1.49, 1.51, 1.53, 1.55, 1.57, 1.59, 1.61, 1.63, 1.65, 1.67, 1.69, 1.71, 1.73, 1.75, 1.77, 1.79, 1.81, 1.83, 1.85, 1.87, 1.89, 1.91, 1.93, 1.95, 1.97, 1.99, 2.01, 2.03, 2.05, 2.07, 2.09, 2.11, 2.13, 2.15, 2.17, 2.19, 2.21, 2.23, 2.25, 2.27, 2.29, 2.31, 2.33, 2.35, 2.37, 2.39, 2.41, 2.43, 2.45, 2.47, 2.49, 2.51, 2.53, 2.55, 2.57, 2.59, 2.61, 2.63, 2.65, 2.67, 2.69, 2.71, 2.73, 2.75, 2.77, 2.79, 2.81, 2.83, 2.85, 2.87, 2.89, 2.91, 2.93, 2.95, 2.97, 2.99, 3.01, 3.03, 3.05, 3.07, 3.09, 3.11, 3.13, 3.15, 3.17, 3.19, 3.21, 3.23, 3.25, 3.27, 3.29, 3.31, 3.33, 3.35, 3.37, 3.39, 3.41, 3.43, 3.45, 3.47, 3.49, 3.51, 3.53, 3.55, 3.57, 3.59, 3.61, 3.63, 3.65, 3.67, 3.69, 3.71, 3.73, 3.75, 3.77, 3.79, 3.81, 3.83, 3.85, 3.87, 3.89, 3.91, 3.93, 3.95, 3.97, 3.99, 4.01, 4.03, 4.05, 4.07, 4.09, 4.11, 4.13, 4.15, 4.17, 4.19, 4.21, 4.23, 4.25, 4.27, 4.29, 4.31, 4.33, 4.35, 4.37, 4.39, 4.41, 4.43, 4.45, 4.47, 4.49, 4.51, 4.53, 4.55, 4.57, 4.59, 4.61, 4.63, 4.65, 4.67, 4.69, 4.71, 4.73, 4.75, 4.77, 4.79, 4.81, 4.85, 4.87, 4.89, 4.91, 4.93, 4.95, 4.97, 4.99, 5.01, 5.03, 5.05, 5.07, 5.09, 5.11, 5.13, 5.15, 5.17, 5.19, 5.21, 5.23, 5.25, 5.27, 5.29, 5.31, 5.33, 5.35, 5.37, 5.39, 5.41, 5.43, 5.45, 5.47, 5.49, 5.51, 5.53, 5.55, 5.57, 5.59, 5.61, 5.63, 5.65, 5.67, 5.69, 5.71, 5.73, 5.75, 5.77, 5.79, 5.81, 5.83, 5.85, 5.87, 5.89, 5.91, 5.93, 5.95, 5.97, 5.99, 6.01, 6.03, 6.05, 6.07, 6.09, 6.11, 6.13, 6.15, 6.17, 6.19, 6.21, 6.23, 6.25, 6.27, 6.29, 6.31, 6.33, 6.35, 6.37, 6.39, 6.41, 6.43, 6.45, 6.47, 6.49, 6.51, 6.53, 6.55, 6.57, 6.59, 6.61, 6.63, 6.65, 6.67, 6.69, 6.71, 6.73, 6.75, 6.77, 6.79, 6.81, 6.83, 6.85, 6.87, 6.89, 6.91, 6.93, 6.95, 6.97, 6.99, 7.01, 7.03, 7.05, 7.07, 7.09, 7.11, 7.13, 7.15, 7.17, 7.19, 7.21, 7.23, 7.25, 7.27, 7.29, 7.31, 7.33, 7.35, 7.37, 7.39, 7.41, 7.43, 7.45, 7.47, 7.49, 7.51, 7.53, 7.55, 7.57, 7.59, 7.61, 7.63, 7.65, 7.67, 7.69, 7.71, 7.73, 7.75, 7.77, 7.79, 7.81, 7.83, 7.85, 7.87, 7.89, 7.91, 7.93, 7.95, 7.97, 7.99}; // bin centers, 20 keV bins
    float Aug28Rarray[nBins] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.775601129, 0.751986759, 0.728276026, 0.67423959, 0.61328776, 0.546116609, 0.485769026, 0.45322076, 0.472411545, 0.53996701, 0.615197996, 0.645330975, 0.616263647, 0.525857339, 0.39447392, 0.267404687, 0.19029124, 0.156317949, 0.148834586, 0.151958417, 0.156470428, 0.156734574, 0.152241268, 0.14375744, 0.133173486, 0.122396241, 0.113152853, 0.106414311, 0.102556995, 0.101197724, 0.101496446, 0.102252038, 0.102530354, 0.101618882, 0.099384983, 0.096054824, 0.09223832, 0.088556156, 0.085533222, 0.083535694, 0.082580576, 0.082536363, 0.08304198, 0.083782834, 0.084333798, 0.084482876, 0.084081556, 0.083182555, 0.08184539, 0.080241245, 0.078534503, 0.076882744, 0.075374962, 0.074096747, 0.073083812, 0.072314616, 0.071709236, 0.071155438, 0.070517181, 0.069648789, 0.068435194, 0.066748291, 0.064528563, 0.061881441, 0.058988312, 0.056123581, 0.053563865, 0.051545364, 0.050085592, 0.049094877, 0.048397839, 0.047800451, 0.047153522, 0.046361369, 0.045409935, 0.044310615, 0.043116405, 0.041894251, 0.040715006, 0.039646918, 0.038702343, 0.037900384, 0.037202836, 0.036589917, 0.03598363, 0.035364078, 0.03467577, 0.033937127, 0.033143273, 0.032330715, 0.031512884, 0.030744085, 0.03002392, 0.029387307, 0.028822059, 0.028333673, 0.027904012, 0.027520604, 0.027149951, 0.026785192, 0.026417146, 0.026031697, 0.025648573, 0.025262105, 0.024895986, 0.024543017, 0.024229387, 0.023945176, 0.023692594, 0.023463252, 0.023242866, 0.023011824, 0.022750173, 0.022454511, 0.022131188, 0.021785249, 0.021434661, 0.021100851, 0.020810008, 0.020552268, 0.020355122, 0.02020066, 0.020087066, 0.02000119, 0.019936684, 0.019877223, 0.019827512, 0.019781825, 0.019745264, 0.019724971, 0.019721457, 0.019738349, 0.019784376, 0.01984395, 0.019915201, 0.019994048, 0.020064336, 0.020125384, 0.02017878, 0.020225714, 0.020260971, 0.02030269, 0.020341235, 0.020376039, 0.02040132, 0.020427621, 0.020452902, 0.020481073, 0.020519845, 0.020563094, 0.020621138, 0.020686438, 0.020758653, 0.020835912, 0.02092961, 0.021022231, 0.02112908, 0.021244147, 0.021370665, 0.021501377, 0.02164354, 0.021783662, 0.021927298, 0.022064359, 0.022200456, 0.022322382, 0.022432858, 0.022518564, 0.022585847, 0.022623315, 0.022639867, 0.022628417, 0.022605913, 0.022565668, 0.022521398, 0.022466755, 0.022416817, 0.02236104, 0.022311896, 0.022259577, 0.022223809, 0.022186171, 0.022162875, 0.022140825, 0.022134419, 0.022129431, 0.022144452, 0.022153465, 0.022168373, 0.022175458, 0.022174891, 0.022164122, 0.022148704, 0.022111973, 0.022062261, 0.022006881, 0.021927184, 0.021836547, 0.021737294, 0.021620696, 0.021489304, 0.021342777, 0.021185083, 0.021016109, 0.020854221, 0.020685758, 0.020529254, 0.020366629, 0.020221463, 0.02007743, 0.019954823, 0.019844687, 0.019758755, 0.019687333, 0.019633484, 0.01958587, 0.019548856, 0.019509404, 0.019476527, 0.019431294, 0.019372513, 0.019291909, 0.019197304, 0.019078439, 0.018949767, 0.018809022, 0.018665896, 0.018517385, 0.018374769, 0.018235724, 0.018110057, 0.017985636, 0.017885703, 0.017796427, 0.017732488, 0.017682266, 0.017657722, 0.017641284, 0.017639243, 0.017643551, 0.017658232, 0.017682946, 0.017713272, 0.017740763, 0.017769559, 0.017794103, 0.017810371, 0.017816946, 0.017816776, 0.017803342, 0.01778356, 0.017756125, 0.017727046, 0.017698648, 0.017671553, 0.017640944, 0.017612999, 0.017588511, 0.017566915, 0.017552007, 0.017545999, 0.017547643, 0.017554558, 0.017564931, 0.017580349, 0.017603362, 0.01763998, 0.017688048, 0.017759412, 0.017838316, 0.017924361, 0.018011768, 0.01809974, 0.018172692, 0.018235951, 0.018292464, 0.018327041, 0.018361052, 0.018383272, 0.018410423, 0.018419776, 0.018429582, 0.018424934, 0.018416431, 0.018392738, 0.018375733, 0.018358558, 0.018341779, 0.018328629, 0.018312587, 0.018292975, 0.018273929, 0.018247118, 0.018217926, 0.018190888, 0.018170878, 0.018145767, 0.018137662, 0.01813205, 0.018135848, 0.01813341, 0.018140836, 0.018145087, 0.018153136, 0.01815427, 0.018163963, 0.018160335, 0.018151606, 0.0181206, 0.01808659, 0.018025768, 0.017955197, 0.017856001, 0.017751193, 0.017626659, 0.01750513, 0.017385811, 0.017270687, 0.017162478, 0.01705954, 0.016950991, 0.016843746, 0.016748404, 0.016658618, 0.016583682, 0.01652116, 0.016460225, 0.016404619, 0.01634187, 0.016277931, 0.01620821, 0.016137752, 0.01605233, 0.015962317, 0.015864878, 0.015761657, 0.015645399, 0.015522452, 0.01539072, 0.015243002, 0.015088483, 0.014937251, 0.014785679, 0.014640343, 0.014495346, 0.014356982, 0.014217937, 0.014084164, 0.013958893, 0.013851591, 0.013755399, 0.01366788,  0.013591584, 0.013511093, 0.013430829, 0.013341382, 0.013253013, 0.013154553, 0.013055867, 0.012945278, 0.012828056, 0.012690372, 0.01255008, 0.012409335, 0.012262411, 0.012125747, 0.011998322, 0.011874809, 0.011750502, 0.0116326, 0.011514925, 0.011413745, 0.011312338, 0.011224592, 0.01114155, 0.011064744, 0.010980739, 0.010901382, 0.010821515, 0.01075083, 0.010679239, 0.010614393, 0.010552268, 0.010487989, 0.010420252}; // counts per s per cm^2 per bin
    float Aug28Parray[nBins] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.67847952, 0.681574443, 0.679125713, 0.656259565, 0.607704429, 0.535418721, 0.45243966, 0.385837046, 0.368652859, 0.404347629, 0.455870716, 0.474427213, 0.44835731, 0.374791688, 0.268507749, 0.16754923, 0.110067, 0.08736977, 0.084638189, 0.089292476, 0.094459182, 0.095880239, 0.093251822, 0.087453094, 0.080288293, 0.073457357, 0.06824247, 0.06512204, 0.064086431, 0.064660636, 0.066155948, 0.067842284, 0.069197588, 0.069821674, 0.069617613, 0.068662495, 0.067184188, 0.065466676, 0.063786575, 0.062408031, 0.061438175, 0.060986407, 0.061003412, 0.061513564, 0.062429004, 0.063694181, 0.065179857, 0.066759061, 0.068190321, 0.069212892, 0.069635185, 0.069348933, 0.06833373, 0.066672902, 0.064599417, 0.062322439, 0.060029022, 0.057880148, 0.055987654, 0.054375687, 0.053027639, 0.051816538, 0.050598692, 0.049306363, 0.047916879, 0.046504438, 0.04519057, 0.04410491, 0.043243887, 0.042534265, 0.041864549, 0.041119557, 0.040238921, 0.03921737, 0.038102688, 0.036938068, 0.035776565, 0.034659559, 0.033609779, 0.032655738, 0.03177958, 0.030986861, 0.030250485, 0.029578218, 0.028917287, 0.02827733, 0.027616796, 0.02695632, 0.026281559, 0.025612693, 0.024941446, 0.024302792, 0.023681767, 0.023106769, 0.022567935, 0.022075582, 0.021626705, 0.021220057, 0.020826616, 0.020441735, 0.020052092, 0.019633711, 0.019197021, 0.018732159, 0.018255734, 0.01776287, 0.017284291, 0.016819826, 0.016381605, 0.015976148, 0.015604247, 0.015260518, 0.01493249, 0.01462056, 0.014324049, 0.014040121, 0.013766622, 0.013510469, 0.013280731, 0.013060969, 0.012866771, 0.012686971, 0.012520038, 0.012359963, 0.012206804, 0.012050641, 0.011897936, 0.011745684, 0.011597229, 0.011458638, 0.01132957, 0.011211498, 0.011110771, 0.011015373, 0.010925132, 0.01084039, 0.010751737, 0.010661214, 0.010574998, 0.010493034, 0.010412996, 0.01034747, 0.010288066, 0.010231609, 0.010172715, 0.010115238, 0.01005487, 0.009994162, 0.009935664, 0.009876146, 0.009821447, 0.009767767, 0.009714712, 0.009662336, 0.009616876, 0.009567901, 0.009523858, 0.009481289, 0.0094411, 0.009400911, 0.009364747, 0.009326826, 0.009291626, 0.009255065, 0.009222188, 0.00918888, 0.009156662, 0.009119704, 0.009081783, 0.009036776, 0.008987745, 0.008930098, 0.008870864, 0.008805734, 0.008739811, 0.00866805, 0.008597422, 0.008522656, 0.008449308, 0.008372558, 0.008302894, 0.008229999, 0.008161582, 0.008091068, 0.008026505, 0.007959902, 0.007900554, 0.007837466, 0.007775964, 0.007709644, 0.007641567, 0.007571563, 0.00750445, 0.007434219, 0.007365802, 0.007303563, 0.007239341, 0.007177612, 0.007118661, 0.007057727, 0.006993958, 0.006926561, 0.006856443, 0.006782414, 0.006710937, 0.006634414, 0.006561292, 0.006481425, 0.006405355, 0.006326112, 0.006251856, 0.006177828, 0.006109807, 0.006044451, 0.005983686, 0.005923829, 0.005869016, 0.005815223, 0.005767495, 0.005718124, 0.005669036, 0.005615385, 0.00556187, 0.005503458, 0.005446026, 0.005387234, 0.005330941, 0.005273895, 0.005219842, 0.005167749, 0.005120022, 0.005070486, 0.005027866, 0.004985925, 0.00494979, 0.004915009, 0.004886837, 0.004858433, 0.004831735, 0.004803915, 0.004777239, 0.004751363, 0.004725918, 0.004698721, 0.004672845, 0.00464664, 0.0046203, 0.004592967, 0.004566824, 0.00453825, 0.004510294, 0.004481209, 0.004453775, 0.004427547, 0.004403258, 0.004377904, 0.004353479, 0.004330153, 0.004307588, 0.004285339, 0.004264911, 0.004245774, 0.004226853, 0.004207553, 0.004188813, 0.004170447, 0.00415482, 0.004140451, 0.004131591, 0.004123842, 0.004117431, 0.004111145, 0.004104269, 0.004092729, 0.004077656, 0.004061428, 0.004038998, 0.004018524, 0.003995658, 0.003977304, 0.003955679, 0.003936866, 0.003915473, 0.003895725, 0.003872779, 0.003853399, 0.003834161, 0.003815659, 0.003797793, 0.003779614, 0.003760081, 0.00374065, 0.003718526, 0.003694827, 0.00367047, 0.003646895, 0.003620146, 0.003596759, 0.003572719, 0.003550012, 0.003524227, 0.003501151, 0.003477944, 0.003457159, 0.003436225, 0.0034199, 0.003402612, 0.003386576, 0.003366663, 0.003348695, 0.00332669, 0.003304958, 0.003277755, 0.003251273, 0.003220635, 0.003191165, 0.003161854, 0.003132934, 0.003105471, 0.00307841, 0.003048946, 0.003019363, 0.002991934, 0.002964817, 0.002940488, 0.002918013, 0.002895283, 0.002872808, 0.002848683, 0.002823947, 0.002799074, 0.002773674, 0.002746505, 0.002718674, 0.002690752, 0.002662631, 0.002633529, 0.002604111, 0.002574743, 0.002542949, 0.002510787, 0.00248003, 0.002450084, 0.002421408, 0.002393446, 0.002366607, 0.002339846, 0.002313664, 0.002289273, 0.002268334, 0.002249719, 0.002232692, 0.002217756, 0.002201493, 0.002184953, 0.002166128, 0.002147451, 0.002126387, 0.002105715, 0.002082554, 0.002058344, 0.002030042, 0.002002296, 0.001974855, 0.001947262, 0.001922542, 0.001900107, 0.001878709, 0.001857594, 0.001837891, 0.001818613, 0.001803824, 0.001789285, 0.001778299, 0.001768544, 0.001760404, 0.001750587, 0.001741869, 0.001732782, 0.001725345, 0.001717506, 0.001710783, 0.001704469, 0.001697293, 0.001689612}; // counts per s per cm^2 per bin
    float Jun30array[nBins] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.179930523, 0.165187729, 0.155206021, 0.135115404, 0.125441845, 0.122297647, 0.123677153, 0.125647875, 0.125753577, 0.123506058, 0.1197375, 0.115353539, 0.111343119, 0.108539319, 0.10748767, 0.108229378, 0.110381048, 0.113092583, 0.11552105, 0.117084191, 0.117367258, 0.116490287, 0.114978206, 0.113550328, 0.11302092, 0.114048383, 0.116666756, 0.120568786, 0.124985891, 0.129046475, 0.132125281, 0.134103169, 0.135144965, 0.135825759, 0.136513721, 0.13734859, 0.137998929, 0.137687196, 0.135595543, 0.131219644, 0.124340032, 0.115383995, 0.105256275, 0.094816821, 0.084998683, 0.076444315, 0.069497699, 0.064137066, 0.060331422, 0.057771185, 0.056135754, 0.055057411, 0.054265091, 0.053536013, 0.052728555, 0.051742118, 0.050558162, 0.049161099, 0.04756356, 0.045812126, 0.043993866, 0.042174262, 0.040404912, 0.038710628, 0.0371001, 0.035578434, 0.034133626, 0.032763257, 0.031466432, 0.030253542, 0.029121631, 0.028085927, 0.027154044, 0.02632437, 0.025599861, 0.024987773, 0.024480132, 0.024066639, 0.023727944, 0.023434485, 0.023161003, 0.022888864, 0.022603109, 0.022310905, 0.022020402, 0.021728915, 0.021433127, 0.021116199, 0.020757349, 0.020332389, 0.019829407, 0.019246521, 0.01860523, 0.017925152, 0.017241401, 0.016586136, 0.015987484, 0.015458883, 0.0150092, 0.014636196, 0.014334317, 0.014095053, 0.013904699, 0.013756179, 0.013637487, 0.013539578, 0.01345206, 0.013370992, 0.013289834, 0.013207243, 0.013122681, 0.013033282, 0.01294021, 0.012838539, 0.012728626, 0.012608502, 0.012478076, 0.012338781, 0.012192858, 0.012043979, 0.011891696, 0.011739592, 0.011584801, 0.011431353, 0.011275308, 0.011118546, 0.010961963, 0.010804843, 0.01064835, 0.010493827, 0.010341007, 0.010187201, 0.010035993, 0.009882993, 0.009728649, 0.009571529, 0.00941199, 0.009246002, 0.00907652, 0.008899871, 0.008719416, 0.008533451, 0.008346044, 0.008156613, 0.007969359, 0.007782911, 0.007598576, 0.007415434, 0.007234647, 0.0070548, 0.006876925, 0.006703152, 0.00653367, 0.006367153, 0.006204891, 0.006046077, 0.00589013, 0.005735984, 0.005584677, 0.005435395, 0.00528966, 0.005145753, 0.005005939, 0.004869252, 0.004736765, 0.004607486, 0.004483268, 0.004362686, 0.004245894, 0.004133205, 0.004023185, 0.003915924, 0.003811735, 0.003710565, 0.003612379, 0.003517632, 0.003425563, 0.003336531, 0.003251298, 0.003169047, 0.003089931, 0.003014453, 0.002940864, 0.002869058, 0.002798694, 0.002730328, 0.00266344, 0.002598899, 0.002536947, 0.002478076, 0.002421516, 0.002367572, 0.002316593, 0.002267289, 0.002219804, 0.00217368, 0.0021289, 0.002085669, 0.002043997, 0.002003857, 0.001965374, 0.001928486, 0.001893004, 0.001858678, 0.001825632, 0.001793751, 0.001762865, 0.001732525, 0.001703537, 0.001675248, 0.001648052, 0.001621869, 0.001596858, 0.001572278, 0.001548674, 0.001526011, 0.001504136, 0.001482888, 0.001462688, 0.001443008, 0.001423811, 0.001405071, 0.001387156, 0.001369795, 0.001353116, 0.001336392, 0.001320124, 0.001304224, 0.00128844, 0.001273185, 0.001258682, 0.001244663, 0.001230626, 0.001216625, 0.001202311, 0.001187969, 0.001173449, 0.001158794, 0.001144416, 0.001130102, 0.001115787, 0.001101589, 0.001087579, 0.001073649, 0.00105989, 0.001046337, 0.001033411, 0.001021004, 0.001008938, 0.000997553, 0.000986767, 0.000976108, 0.000965708, 0.000955326, 0.000944925, 0.000934409, 0.00092374, 0.000913, 0.000902501, 0.000892018, 0.000881765, 0.000871769, 0.000862015, 0.000852569, 0.000843463, 0.000834744, 0.000826302, 0.000818237, 0.0008103, 0.000802585, 0.000794822, 0.000787313, 0.000779678, 0.000772098, 0.000764687, 0.000757638, 0.000750591, 0.000744028, 0.000737881, 0.000732114, 0.000726513, 0.000721367, 0.000716191, 0.000711159, 0.000705937, 0.000700615, 0.000695224, 0.000689852, 0.000684604, 0.000679484, 0.000674778, 0.000670285, 0.000666153, 0.00066229, 0.00065882, 0.000655448, 0.000652276, 0.000649225, 0.00064632, 0.000643442, 0.000640631, 0.000637909, 0.000635381, 0.000632992, 0.000630815, 0.000629096, 0.000627817, 0.00062686, 0.000626236, 0.000625931, 0.000625729, 0.000625561, 0.000625305, 0.00062499, 0.000624561, 0.000624065, 0.000623423, 0.000623023, 0.000622559, 0.000622304, 0.000622171, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // counts per s per cm^2 per bin
       Aug28P = new TH1F("BGAug30P","August 28P BG gamma spectrum", nBins, 0, 8*MeV);
       Aug28R = new TH1F("BGAug30P","August 28R BG gamma spectrum", nBins, 0, 8*MeV);
       Jun30 = new TH1F("BGAug30P","June 30 BG gamma spectrum", nBins, 0, 8*MeV);
       for(unsigned int i=0; i<nBins; i++) {
	 Aug28R->Fill(ebins[i],Aug28Rarray[i]);
	 Aug28P->Fill(ebins[i],Aug28Parray[i]);
	 Jun30->Fill(ebins[i],Jun30array[i]);
    }
}

void SimpleBGModule::GeneratePrimaries(G4Event* anEvent) {    
    primaryPtcl p;
    p.PDGid = 22;
    p.KE = Aug28R->GetRandom();
    p.t = 0;
    
    vector<primaryPtcl> v;
    v.push_back(p);
    setVertices(v);
    throwPrimaries(v,anEvent);
}

void SimpleBGModule::fillNode(TXMLEngine& E) {
    addAttr(E, "rate", netRate);
    //addAttr(E, "t_frac", double(nHits)/double(nSurfaceThrows));
}
