#pragma once

typedef struct FLTPOLYGON {
	unsigned int sfltPolyCreatorPrimeColor;		// Creator Primary Color Index - encodes primary color index and intensity
	unsigned int sfltPolyPrimeColor;				// Primary Color Index
												//(0 - 1023) - (without intensity) in index mode
	float sfltPolyPrimeIntensity;				//Primary Color Intensity
												//(0.0 - 1.0) - in index mode
	unsigned int sfltPolyCreatorAltColor;		// Creator Alternate Color Index - encodes alternate color index and intensity
	unsigned int sfltPolyAltColor;				// Alternate Color Index
												//(0 - 1023) - (without intensity) in index mode
	float sfltPolyAltIntensity;					// Alternate Color Intensity
												//(0.0 - 1.0) - in index mode
	short sfltPolyMaterial;						// Material index(-1 if none)
	short sfltPolyShader;						// Shader index(-1 if none)
	short sfltPolyTexture;						// Texture pattern index(-1 if none)
	short sfltPolyDetailTexture;					// Detail texture pattern index(-1 if none)
	short sfltLayerTexture1;						// Texture pattern index for layer 1 (-1 if none)
	short sfltLayerTexture2;						// Texture pattern index for layer 2 (-1 if none)
	short sfltLayerTexture3;						// Texture pattern index for layer 3 (-1 if none)
	short sfltLayerTexture4;						// Texture pattern index for layer 4 (-1 if none)
	short sfltLayerTexture5;						// Texture pattern index for layer 5 (-1 if none)
	short sfltLayerTexture6;						// Texture pattern index for layer 6 (-1 if none)
	short sfltLayerTexture7;						// Texture pattern index for layer 7 (-1 if none)
	short sfltPolyTexmap;						// Texture mapping index(-1 if none)
	short sfltLayerTexmap1;						// Texture mapping index for layer 1 (-1 if none)
	short sfltLayerTexmap2;						// Texture mapping index for layer 2 (-1 if none)
	short sfltLayerTexmap3;						// Texture mapping index for layer 3 (-1 if none)
	short sfltLayerTexmap4;						// Texture mapping index for layer 4 (-1 if none)
	short sfltLayerTexmap5;						// Texture mapping index for layer 5 (-1 if none)
	short sfltLayerTexmap6;						// Texture mapping index for layer 6 (-1 if none)
	short sfltLayerTexmap7;						// Texture mapping index for layer 7 (-1 if none)
	short sfltLayerEffect1;						// MultiTexture effect for layer 1
	short sfltLayerEffect2;						// MultiTexture effect for layer 2
	short sfltLayerEffect3;						// MultiTexture effect for layer 3
	short sfltLayerEffect4;						// MultiTexture effect for layer 4
	short sfltLayerEffect5;						// MultiTexture effect for layer 5
	short sfltLayerEffect6;						// MultiTexture effect for layer 6
	short sfltLayerEffect7;						// MultiTexture effect for layer 7
	short sfltLayerData1;						// MultiTexture data for layer 1
	short sfltLayerData2;						// MultiTexture data for layer 2
	short sfltLayerData3;						// MultiTexture data for layer 3
	short sfltLayerData4;						// MultiTexture data for layer 4
	short sfltLayerData5;						// MultiTexture data for layer 5
	short sfltLayerData6;						// MultiTexture data for layer 6
	short sfltLayerData7;						// MultiTexture data for layer 7
	int sfltPolyInfrared;						// IR Color Code
	int sfltPolyIrMaterial;						// IR Material code
	short sfltPolyPrio;							// Polygon relative priority
	char sfltPolyDrawType;						// How to draw the polygon
												//0 = Draw solid(front side only)
												//1 = Draw solid(both sides visible)
												//2 = Draw closed wireframe
												//3 = Draw wireframe(unclosed)
	char sfltPolyTexWhite;						// Draw Textured Polygon White - TRUE / FALSE
	char sfltPolyTemplate;						// Set template transparency
												//0 = None
												//1 = Fixed
												//2 = Axis type rotate
												//4 = Point rotate
	short sfltPolySmc;							// Surface material code(for DFAD)
	short sfltPolyFid;							// Feature ID(for DFAD)
	unsigned short sfltPolyTransparency;			// Transparency
												//0 = opaque
												//0xffff = fully transparent
	unsigned char sfltPolyLodControl;			// Influences LOD Generation
	unsigned char sfltPolyLineStyle;				// Linestyle Index
	mgbool sfltPolyTerrain;						// TRUE if geometry is terrain
	mgbool sfltPolyNoPrimeColor;					// TRUE if there is no primary color
	mgbool sfltPolyNoAltColor;					// TRUE if there is no alternate color
	mgbool sfltPolyRgbMode;						// TRUE if color is RGB mode(read only)
	mgbool sfltPolyFootprint;					// TRUE if geometry is a footprint for CAT cutout
	mgbool sfltPolyHidden;						// TRUE if geometry is hidden - not drawn
	mgbool sfltPolyRoofline;						// TRUE if geometry is part of a roofline as seen from above
	unsigned char sfltGcLightMode;				// Polygon lighting mode
												//0 = None(Flat)
												//1 = Gouraud
												//2 = Dynamic(Lit)
												//3 = Dynamic Gouraud(Lit Gouraud)
	char* sfltPolySmartPalette[128];					// Smart palette name
	mgbool sfltIPln;								// Polygon is BSP polygon - TRUE / FALSE, a polygon with this flag set will be drawn in the graphics view as the BSP polygon.
												//This flag is valid at runtime only and is not saved with the OpenFlight database file
	char sfltIOn;								// Node Displayed - TRUE / FALSE, this flag is valid at runtime only and is not saved with the OpenFlight database file
	char* sfltComment;							// Comment Text
} polygon_pro;