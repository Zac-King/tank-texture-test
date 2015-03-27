#include "Core.h"
#include "System.h"
#include "DependentClear.h"
#include "ccollisionutils.h"

int main()
{
	coreLoad(Window);
	coreLoad(Time);
	coreLoad(Input);
	coreLoad(Camera);
	coreLoad(Factory);
	coreLoad(Asset);

	systemLoad(TransformUpdate);
	systemLoad(Rendering);
	systemLoad(TextRenderSys);
	systemLoad(CollisionPrep);
	systemLoad(CollisionDetection);
	systemLoad(StaticCollisionResponse);
	systemLoad(DynamicCollisionResponse);
	systemLoad(collisionRender);

	systemLoad(TankDynamics);
	systemLoad(RigidbodyDynamics);

	systemLoad(LifeTimeDynamics);

	initCore();

	////////////////////////////////////////////////////////////////////////////////////////
	////// Shader
	core(Asset).loadShader("Flat", "./shaders/simpleShader.vert", "./shaders/simpleShader.frag");
	core(Asset).loadShader("Texture", "./shaders/simpleShader.vert", "./shaders/textureShader.frag");
	core(Asset).loadShader("FontShader", "./shaders/simpleShader.vert", "./shaders/fontShader.frag");
	////// Textures
	core(Asset).loadTexture("Tank_Base", "./textures/red_tank_base.png", 1, 1);
	core(Asset).loadTexture("Tank_Turret", "./textures/red_tank_turret.png", 1, 1);
	core(Asset).loadTexture("BulletT", "./textures/Bullet.png", 1, 1);
	core(Asset).loadTexture("Leaf", "./textures/leaf.jpg", 1, 1);
	core(Asset).loadTexture("Finger", "./textures/FINGER.png", 1, 5);
	core(Asset).loadTexture("Font", "./textures/font2.png", 8, 16);
	//////////////////////////////// Geometry //////////////////////////////////////////////
	Vertex Anime[4] = {
					{ { -70, -70, 0, 1 }, { 0, 1, 1, 0 }, { 0, 1 } },   
					{ {  70, -70, 0, 1 }, { 0, 1, 1, 0 }, { 1, 1 } },   
					{ {  70,  70, 0, 1 }, { 0, 1, 1, 0 }, { 1, 0 } },   
					{ { -70,  70, 0, 1 }, { 0, 1, 1, 0 }, { 0, 0 } }, };
	////// Tank 1
	Vertex Tank1[4] = {
					{ { -30, -30, 0, 1 }, { 0, 1, 1, 0 }, { 0, 1 } },    // TL
					{ {  30, -30, 0, 1 }, { 0, 1, 1, 0 }, { 1, 1 } },    // TR
					{ {  30,  30, 0, 1 }, { 0, 1, 1, 0 }, { 1, 0 } },    // BR
					{ { -30,  30, 0, 1 }, { 0, 1, 1, 0 }, { 0, 0 } }, }; // BL
	////// Tank 2
	Vertex Tank2[4] = {
					{ { -30, -30, 0, 1 }, { 0, 0, 6, 0 }, { 0, 1 } },    // TL
					{ {  30, -30, 0, 1 }, { 0, 0, 6, 0 }, { 1, 1 } },    // TR
					{ {  30,  30, 0, 1 }, { 0, 0, 6, 0 }, { 1, 0 } },    // BR
					{ { -30,  30, 0, 1 }, { 0, 0, 6, 0 }, { 0, 0 } }, }; // BL
	//////// Turret
	Vertex Turret[4] = {
					{ { -25, -25, 0, 1 }, { 0, 1, 1, 0 }, { 0, 1 } },    // TL
					{ {  25, -25, 0, 1 }, { 0, 1, 1, 0 }, { 1, 1 } },    // TR
					{ {  25,  25, 0, 1 }, { 0, 1, 1, 0 }, { 1, 0 } },    // bR
					{ { -25,  25, 0, 1 }, { 0, 1, 1, 0 }, { 0, 0 } }, }; // BL
	////// Bullet
	Vertex Bullet[3] = {
					{ {  0, 15, 0, 1 }, { 1, 1, 1, 0 } },
					{ { -5, -5, 0, 1 }, { 0, 1, 0, 1 } },
					{ {  5, -5, 0, 1 }, { 1, 0, 1, 0 } } };

	Triangle tank1[2] = { { 0, 1, 2 }, { 0, 2, 3 } };
	Triangle tank2[2] = { { 0, 1, 2 }, { 0, 2, 3 } };
	Triangle turret[2] = { { 0, 1, 2 }, { 0, 2, 3 } };
	Triangle bullet[1] = { 0, 1, 2 };
	//////// Test
	Vertex QUAD[4] = {
					{ { -.5, -.5, 0, 1 }, { 0, 1, 1, 0 }, { 0, 1 } },    // TL
					{ {  .5, -.5, 0, 1 }, { 0, 1, 1, 0 }, { 1, 1 } },    // TR
					{ {  .5,  .5, 0, 1 }, { 0, 1, 1, 0 }, { 1, 0 } },    // BR
					{ { -.5,  .5, 0, 1 }, { 0, 1, 1, 0 }, { 0, 0 } }, }; // BL
	Triangle tQUAD[2] = { { 0, 1, 2 }, { 0, 2, 3 } };
	////////////////////////////////////////////////////////////////////////////////////////
	////// Mesh
	core(Asset).loadMesh("Tank1", Tank1, 4, tank1, 2);
	core(Asset).loadMesh("Tank2", Tank2, 4, tank2, 2);
	core(Asset).loadMesh("Turret", Turret, 4, turret, 2);
	core(Asset).loadMesh("Bullet", Bullet, 3, bullet, 1);
	core(Asset).loadMesh("Anime", Anime, 4, tank1, 2);
	core(Asset).loadMesh("Quad", QUAD, 4, tQUAD, 2);
	//core(Asset).loadMesh("QUAD", QUAD, 4, QUAD, 2); /*Test Mesh*/
	////////////////////////////////////////////////////////////////////////////////////////
	core(Factory).BGImage(75, core(Window).height()-75);
	core(Factory).Maketext(400, 600, "Tank Battle", 25, 45);
	////// Make Borders				4 Planes
	core(Factory).CreatePlane(0, core(Window).height() / 2, vec3Make(1, 0, 0), 0);
	core(Factory).CreatePlane(core(Window).width(), core(Window).height() / 2, vec3Make(-1, 0, 0), 0);
	core(Factory).CreatePlane(core(Window).width() / 2, 0, vec3Make(0, 1, 0), 0);
	core(Factory).CreatePlane(core(Window).width() / 2, core(Window).height(), vec3Make(0, -1, 0), 0);
	////// Make Player objects		2 Tanks

	core(Factory).MakeTank(core(Window).width() / 2 - 320, core(Window).height() / 2 + 50, 1);
	//core(Factory).MakeTank(core(Window).width() / 2 - 320, core(Window).height() / 2 - 50, 2, 'I', 'K', 'J', 'L', 'U', 'O', 'N');

	////// Static Obsticale Test
	//core(Factory).CreateCircle((core(Window).width() / 3) + 75, core(Window).height() / 2, 55, 0);
	//core(Factory).CreateCircle( core(Window).width()      - 75, core(Window).height() / 2, 55, 0);
	core(Factory).CreateRect(core(Window).width() / 2, core(Window).height() / 2, 0);
	
	
	////////////////////////////////////////////////////////////////////////////////////////

	////// Texture draw Test
	//core(Factory).AddImage(100, 100, 100, 100, "Tank_base");
	//core(Factory).AddImage(100, 200, "leaf");
	//core(Factory).AddImage(100, 300, "Tank_turret");
	//core(Factory).AddImage(100, 400, "Tank_turret");
	//core(Factory).AddImage(100, 500, "Tank");
	//core(Factory).AddImage(100, 600, "Tank");



	runGame();
}