
# include "Shape.h"
# include "Vector.h"
# include "PropertyVolume.h"
# include "ScalarVolumeXform.h"
# include "Operations.h"

// ===================================== setup ========================================

// constant setup
Color redColor(1.0, 0.0, 0.0, 1.0);
Color greenColor(0.0, 1.0, 0.0, 1.0);
Color whiteColor(1.0, 1.0, 1.0, 1.0);
Color greyColor(0.1, 0.1, 0.1, 1.0);
Color yellowColor(1.0, 1.0, 0.0, 1.0);
Color brownColor(0.54, 0.27, 0.074, 1.0);
Color blueColor(0.0, 0.0, 0.5, 1.0);
Colorful colorfulVolume;
ConstantColor red(redColor);
ConstantColor green(greenColor);
ConstantColor white(whiteColor);
ConstantColor grey(greyColor);
ConstantColor brown(brownColor);
ConstantColor yellow(yellowColor);
ConstantColor blue(blueColor);

ConstantFloat consRho_1(1.0);
ConstantFloat consRho_2(0.8);
ConstantFloat consRho_3(0.5);
ConstantFloat consRho_4(0.2);
ConstantFloat consRho_0(10.0);

// ------------------------------------------------------------------------------------

// volume setup
// head
Sphere head1(1.3);
Box box1(10.0, 6);
ScalarTranslate cutoutbox1(box1, Vector(0.0, -1.3, 0.0));
ImplicitCutout uphead(head1, cutoutbox1);

ColorVolume upColor(green, uphead);
DensityVolume upDensity(consRho_3, uphead);
// KVolume upK(1, uphead);

//lowhead
Box box2(0.8, 6);
ScalarTranslate cutoutbox2(box2, Vector(0.0, -1.6, 0.0));
ImplicitIntersec lowhead1(box2, cutoutbox2);
ScalarTranslate lowhead(lowhead1, Vector(0.0, 0.3, 0.0));

ColorVolume lowColor(green, lowhead);
DensityVolume lowDensity(consRho_3, lowhead);
// KVolume lowK(1, lowhead);
// --------------------------------------------
// left eye
Ellipse eye1(0.5, 0.4, Vector(0.0, 1.0, 0.0));
ScalarTranslate eye2(eye1, Vector(0.85, 0.75, 0.5));
ImplicitCutout lefteye(eye2, uphead);
// right eye
ScalarTranslate eye3(eye1, Vector(0.85, 0.75, -0.5));
ImplicitCutout righteye(eye3, uphead);

ImplicitUnion eyes(lefteye, righteye);
ColorVolume eyesColor(white, eyes);
DensityVolume eyesDensity(consRho_0, eyes);
// KVolume eyesK(10, eyes);
// --------------------------------------------
// upbody
// up
Cone body1(23.0, 2.0, Vector(0.0, -1.0, 0.0), Vector(0.0, 1.1, 0.0));
Box box3(40.0, 8);
ScalarScaling body2(body1, 1.9);
ScalarScaling box4(box3, 2.0);
ScalarTranslate cutoutbox3(box4, Vector(0.0, 1.6, 0.0));
ImplicitCutout upbody1(body2, cutoutbox3);
// left arm & right arm
Sphere shoulder1(1.5);
Ellipse arm00(3.5 , 1.0, Vector(0.0, 1.0, 0.0));
ScalarTranslate arm_0(arm00, Vector(0.0, -3.0, 0.0));
ImplicitBlinnBlend arm0(arm_0, shoulder1);
ScalarScaling arm1(arm0, 0.5);
// left arm
ScalarRotation arm2(arm1, Vector(0.0, 0.0, 1.0), 45.0);
ScalarRotation arm3(arm2, Vector(1.0, 0.0, 0.0), 45.0);
ScalarTranslate arm4(arm3, Vector(0.0, -1.0, -1.3));
// right arm
ScalarRotation arm22(arm1, Vector(0.0, 0.0, 1.0), 45.0);
ScalarRotation arm33(arm22, Vector(1.0, 0.0, 0.0), -45.0);
ScalarTranslate arm44(arm33, Vector(0.0, -1.0, 1.3));
ImplicitUnion arms(arm4, arm44);
ImplicitUnion upbody(arms, upbody1);

ColorVolume upbodyColor(green, upbody);
DensityVolume upbodyDensity(consRho_3, upbody);
// KVolume upbodyK(2, upbody);
// ----------------------------------------------
// mouth
ScalarScaling mouth1(upbody1, 0.4);
ScalarTranslate mouth(mouth1, Vector(0.6, 0.1, 0.0));

ColorVolume mouthColor(red, mouth);
DensityVolume mouthDensity(consRho_1, mouth);
// KVolume mouthK(1, mouth);
// -----------------------------------------------
// lowbody
Ellipse leg1(3.0, 0.8, Vector(0.0, 1.0, 0.0));
// left leg & right leg
ScalarTranslate leftleg(leg1, Vector(0.0, -2.0, 0.6));
ScalarTranslate rightleg(leg1, Vector(0.0, -2.0, -0.6));
ImplicitUnion leftrightleg1(leftleg, rightleg);
ScalarTranslate cutoutbox4(cutoutbox3, Vector(0.0, -1.0, 0.0));
ImplicitCutout leftrightleg(leftrightleg1, cutoutbox4);
// hip
Sphere hip1(1.8);
ScalarTranslate hip2(hip1, Vector(0.0, -1.4, 0.0));
ImplicitCutout hip(hip2, cutoutbox4);
ImplicitUnion leg(leftrightleg, hip);

ColorVolume legColor(blue, leg);
DensityVolume legDensity(consRho_3, leg);
// KVolume legK(2, leg);
// ------------------------------------------------
// belt
Torus belt1(1.4, 0.22, Vector(0.0, 1.0, 0.0));
ScalarTranslate belt(belt1, Vector(0.0, -1.8, 0.0));
ColorVolume beltColor(red, belt);
DensityVolume beltDensity(consRho_1, belt);
// KVolume beltK(1, belt);

// button
SteinerPatch button1;
ScalarTranslate button(button1, Vector(0.0, -4.0, 2.0));

ColorVolume buttonColor(yellow, button);
DensityVolume buttonDensity(consRho_0, button);
// KVolume buttonK(10, button);


// flower
Icosahedron flower1;
ScalarScaling flower2(flower1, 0.18);
ScalarTranslate flower(flower2, Vector(0.0, -4.0, -2.0));

ColorVolume flowerColor(yellow, flower);
DensityVolume flowerDensity(consRho_3, flower);
// KVolume flowerK(0.5, flower);

// prodium
ScalarScaling prodium1(lowhead1, 2.0);
ScalarTranslate prodium(prodium1, Vector(0.0, -4.0, 0.0));
ColorVolume prodiumColor(blue, prodium);
DensityVolume prodiumDensity(consRho_4, prodium);
// KVolume prodiumK(0.1, prodium);
// ------------------------------------------------------------------------------------

// color add
// head color
ColorAdd headc(upColor, lowColor);
ColorAdd headallc(headc, eyesColor);

ColorAdd bodycc(legColor, upbodyColor);
ColorAdd bodyc(bodycc, beltColor);
ColorAdd beltc(beltColor, prodiumColor);
ColorAdd color1(bodyc, beltc);
ColorAdd color2(color1, mouthColor);
ColorAdd color3(color2, flowerColor);
ColorAdd color4(color3, buttonColor);

ColorAdd humanFinalColor(color4, headallc);

// density add
FloatAdd headd(upDensity, lowDensity);
FloatAdd headalld(headd, eyesDensity);

FloatAdd bodydd(legDensity, upbodyDensity);
FloatAdd bodyd(bodydd, beltDensity);
FloatAdd beltd(beltDensity, prodiumDensity);
FloatAdd dens1(bodyd, beltd);
FloatAdd dens2(dens1, mouthDensity);
FloatAdd dens3(dens2, flowerDensity);
FloatAdd dens4(dens3, buttonDensity);

FloatAdd humanFinalDensity(dens4, headalld);

