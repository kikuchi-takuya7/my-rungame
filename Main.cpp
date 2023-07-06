# include <Siv3D.hpp> // OpenSiv3D v0.6.3

#define NUM 3

Vec2 playerPos = { 200, 480 };//プレイヤーの位置

//最初だけ呼び出すやつ&リスタートに使う
void InitAll(double &_move, double &_move2, double &_move3, int &_score, int &_die, double &_tMove, double &_limit,double &_limitcount,
				double &_velocity, double &_gravity, int _pattern[NUM], int &_period, int &_jumpcount, int &_jumptmp, int &_speadtmp);

//タイトル
void Title(Font _font, String _title, String _please, Texture _backtitle);

//ほぼすべての処理
void UpdateAll(int &_period,double &_tMove,double &_move, double &_move2, double &_move3, double &_limit,double &_limitcount, int &_score,
				int &_die, double &_velocity, double &_gravity, int &_jumptmp, int &_jumpcount, Font _speadup, String _Up, int &_speadtmp);

//背景の表示を一番後ろに
void DrawBack(Texture _background, double _move, double _move2, double _move3);

//ほとんどすべての表示
void DrawAll(double _move, double _move2, double _move3, int _die, double _limit, Font _font, String _gameclears, String _gameovers,
				RectF _scaffold, RectF _edge, Circle _player, Texture _explosion, String _restart);

//オブジェの上に乗ったときに重力などをなくす関数
void Reset(double& _velocity, double& _gravity, int& _jummptmp, int& _jumpcount);

//地面にくっついてる100×100オブジェクトの当たり判定
void CollisionO(double _tMove, double &_velocity, double &_gravity, int &_jumptmp, int &_jumpcount,
				Circle _player, RectF _object, RectF objectsub);

//空中に浮いてる正方形オブジェの当たり判定。下からあたったときの挙動が少し磁石みたいになってしまっているが、修正する時間がない
void CollisionOSky(double _tMove, double& _velocity, double& _gravity, int& _jumptmp, int& _jumpcount,
				Circle _player, RectF _object, RectF objectsub/*, RectF objectunder*/);

//敵の当たり判定
void CollisionE(int &_die, Circle _player, Triangle _enemy);

//敵が出てくるパターン
void Pattern0(double _move2, double _move3, double _bottomO, double _bottomE, double _scaleE, Circle _player, int &_die,
				double _tMove, Texture _mob, Texture _skymob, double &_velocity, double &_gravity, int &_jumptmp, int &_jumpcount);

void Pattern1(double _move2, double _move3, double _bottomO, double _bottomE, double _scaleE, Circle _player, int& _die,
				double _tMove, Texture _mob, Texture _skymob, double& _velocity, double& _gravity, int& _jumptmp, int& _jumpcount);

void Pattern1Draw(Texture _skymob, double _move2, double _move3, double _bottomE, RectF _object, RectF _objectsub, RectF _object2,
					RectF _object2sub, RectF _object3, RectF _object3sub, RectF _object9, RectF _object9sub);

void Pattern2(double _move2, double _move3, double _bottomO, double _bottomE, double _scaleE, Circle _player, int& _die,
				double _tMove, Texture _mob, Texture _skymob, double& _velocity, double& _gravity, int& _jumptmp, int& _jumpcount);

void Pattern2Draw(Texture _mob, double _move2, double _move3, double _bottomE, RectF _object, RectF _objectsub, RectF _object2,
					RectF _object2sub, RectF _object3, RectF _object3sub, RectF _object4, RectF _object4sub, RectF _object5, RectF _object5sub);

void Main()
{
	//テキスト一覧
	const Texture background{ U"example/sky002_day.jpg" };
	const Texture explosion{ U"example/explosion.png" };
	const Texture mob{ U"example/mob.png" };
	const Texture skymob{ U"example/skymob.png" };
	const Texture backtitle{ U"example/title.jpg" };
	const Font minifont{ FontMethod::SDF, 40 };
	const Font font{ FontMethod::SDF, 80 };
	const Font bigfont{ FontMethod::SDF, 120 };
	const String restart = U"RESTART:SPACE";
	const String gameclears = U"GAME CLEAR";
	const String gameovers = U"GAME OVER";
	const String Up = U"SPEAD UP!";
	const String title = U"Escape\n  Run";
	const String please = U"Please Space";
	//最初の足場
	RectF scaffold{ 0, 500, 800, 10 };
	//画面端の表示
	RectF edge{ 0, 0, 5, 800 };
	double move;//背景やオブジェクトの移動すべての移動速度
	double move2;//
	double move3;//
	int score;//スコア　
	const double bottomO = 400.0;//修正がしやすいように障害物のy座標
	const double bottomE = 480.0;//敵のy座標
	const double scaleE = 50;//敵の大きさ
	int die;//死亡時に画面を止める変数
	double tMove;//時間を獲得する変数　(Scene::DeltaTime() * 180);
	double limit;//時間制限
	double limitcount;
	double velocity;//ジャンプの初速
	double gravity;//重力
	int pattern[NUM]; //オブジェクトの配置パターン
	int period;//パターンの周期
	int jumpcount;//ジャンプ回数
	int jumptmp;//1ならジャンプの処理が続いて0なら処理しない・時間があれば関数にしたい
	int speadtmp;//スピードアップを表示させるためのやつ

	//初期化
	InitAll(move, move2, move3, score, die, tMove,limit, limitcount , velocity, gravity, pattern, period, jumpcount, jumptmp, speadtmp);

	//タイトルの表示
	Title(minifont, title, please, backtitle);

	while (System::Update())
	{
		ClearPrint();

		//背景の表示
		DrawBack(background, move, move2, move3);

		UpdateAll(period, tMove, move, move2, move3, limit,limitcount, score, die, velocity, gravity, jumptmp, jumpcount, font, Up, speadtmp);

		//プレイヤーの宣言
		Circle player{ playerPos.x, playerPos.y, 20 };

		//randでいくつかのパターンの中からランダムで選ばれるようにした
		switch (pattern[period])
		{
		case 0:

			if (limit >= 0) {
				Pattern0(move2, move3, bottomO, bottomE, scaleE, player, die, tMove,
						 mob, skymob, velocity, gravity, jumptmp, jumpcount);
			}

			break;

		case 1:

			if (limit >= 0) {
				Pattern1(move2, move3, bottomO, bottomE, scaleE, player, die, tMove,
						 mob, skymob, velocity, gravity, jumptmp, jumpcount);
			}

			break;

		case 2:

			if (limit >= 0) {
				Pattern2(move2, move3, bottomO, bottomE, scaleE, player, die, tMove,
						 mob, skymob, velocity, gravity, jumptmp, jumpcount);
			}

			break;
		}
		
		DrawAll(move, move2, move3, die, limit, font, gameclears, gameovers, scaffold, edge, player, explosion, restart);

		if (die != 0 && KeySpace.down()) {
			InitAll(move, move2, move3, score, die, tMove, limit,limitcount, velocity, gravity, pattern, period, jumpcount, jumptmp, speadtmp);
			playerPos = { 200, 480 };
		}

	}
}

void InitAll(double &_move, double &_move2, double &_move3, int &_score, int &_die, double &_tMove, double &_limit,double &_limitcount,
				double &_velocity, double &_gravity, int _pattern[NUM], int &_period, int &_jumpcount, int &_jumptmp, int &_speadtmp){

	_move = 0.0;
	_move2 = 800.0;
	_move3 = 1600.0;
	_score = 0;
	_die = 0;
	_tMove = 0.0;
	_limit = 30.0;
	_limitcount = 0.0;
	_velocity = 0;
	_gravity = 0;
	_pattern[NUM];
	_period = 0;
	_jumpcount = 0;
	_jumptmp = 0;
	_speadtmp = 0;

	//リスタートされるたびパターンが変わる
	srand(time(NULL));

	for (int i = 0; i < NUM; i++) {
		_pattern[i] = rand() % 3;
	}
}

void Title(Font _font, String _title, String _please, Texture _backtitle)
{
	while (System::Update())
	{

		_backtitle.scaled(1.7).draw(0, 0);

		_font(_title).draw(TextStyle::Outline(0.2, { Palette::Red }), 120, 200, 50);

		_font(_please).draw(TextStyle::Outline(0.2, { Palette::Red }), 270, 500);
		

		if (KeySpace.down())
			break;
	}
}



void UpdateAll(int &_period, double &_tMove,double &_move, double &_move2, double &_move3,double &_limit, double &_limitcount, int &_score,/*, double _bottomO, double _bottomE, double _scaleE, Vec2 _player*/
				int &_die, double &_velocity, double &_gravity, int &_jumptmp, int &_jumpcount, Font _font, String _Up, int &_speadtmp){

	//スピードアップ
	if (_period == 0) {
		_tMove = (Scene::DeltaTime() * 180);
	}
	else if (_period == 1) {
		_tMove = (Scene::DeltaTime() * 240);
	}
	else if (_period == 2) {
		_tMove = (Scene::DeltaTime() * 300);
	}


	//背景を移動する
	if (_die == 0 && _limit >= 0) {
		_move -= _tMove;
		_move2 -= _tMove;
		_move3 -= _tMove;
		if (_move <= -1600) {
			_move = 800.0;
			_speadtmp = 1;
		}
		if (_move2 <= -1600) {
			_move2 = 800.0;
			_period += 1;//周期を一回終えて素早さアップ
			
		}
		if (_move3 <= -1600) {
			_move3 = 800.0;

		}
	}


	if (_limit >= 5 && _die == 0) {
		if (_speadtmp == 1) {
			_font(_Up).draw(_move, 200);
		}
	}


	//時間計測
	if (_die == 0 && _limit >= 0) {
		_limitcount += Scene::DeltaTime();
		_limit = 30 - _limitcount;
		Print << U"Time:" << _limit;
	}
	else if (_die != 0) {
		Print << U"Time:" << _limit;
	}

	//スコアの増加と表示
	if (_die == 0 && _limit >= 0) {
		_score += _tMove;
		_score += playerPos.x * 0,1;//高得点を目指せるようにプレイヤーの位置分の得点を加算するようにした
	}

	Print << U"score:" << _score;

	//ジャンプ
	if (KeySpace.down() && _jumpcount <= 1 && _die == 0) {
		_velocity = 8.0f; //ジャンプの初速
		_gravity = 2.0f; //重力 
		_jumpcount += 1;
		_jumptmp = 1;
	}

	if (_jumptmp == 1) {
		playerPos.y -= _velocity;
		playerPos.y += _gravity;
		_gravity += 0.1f;
	}

	//急降下
	if (KeyDown.down() && _jumptmp != 0) {
		_gravity = 30.0f;
	}

	if (playerPos.y >= 480) {
		playerPos.y = 480;
		Reset(_velocity, _gravity, _jumptmp, _jumpcount);
	}

	//押し戻されて画面端に言ったら画面が止まる
	if (playerPos.x <= 20) {
		_die += 1;
	}
}

void DrawBack(Texture _background, double _move, double _move2, double _move3){

	//背景の表示
	_background.draw(_move, 0);
	_background.draw(_move2, 0);
	_background.draw(_move3, 0);

}

void DrawAll(double _move, double _move2, double _move3, int _die, double _limit, Font _font, String gameclears, String gameovers,
				RectF _scaffold, RectF _edge, Circle _player, Texture _explosion, String _restart){

	//30秒経過したらクリア
	if (_limit <= 0) {
		_font(gameclears).draw(TextStyle::Outline(0.2, { Palette::Red }), 20, 200);
	}

	//足場
	_scaffold.draw(Palette::Black);

	//画面端
	_edge.draw(Palette::Red);

	//プレイヤーが死んだら表示しない
	if (_die == 0) {
		_player.draw(Palette::Black);
	}

	//爆発のエフェクト
	if (_die >= 1 && _die <= 120) {
		_explosion.draw(playerPos.x - 400, playerPos.y - 300);
	}
	else if (_die >= 120 && _die <= 300) {

		_explosion.draw(playerPos.x - 400, playerPos.y - 300, ColorF{ 1.0, Periodic::Sine0_1(4s) });
	}
	//ゲームオーバー表示
	if (_die > 1)
	{
		_font(gameovers).draw(TextStyle::Outline(0.2, { Palette::Red }), 20, 200);
		_font(_restart).draw(TextStyle::Outline(0.2, { Palette::Red }), 50, 200, 500);
	}
}


void Reset(double& _velocity, double& _gravity, int& _jumptmp, int& _jumpcount) {

	_velocity = 0;
	_gravity = 0;
	_jumptmp = 0;
	_jumpcount = 0;
}

void CollisionO(double _tMove, double& _velocity, double& _gravity, int& _jumptmp, int& _jumpcount,
				Circle _player, RectF _object, RectF _objectsub){

	/*オブジェクトに当たったら、押し戻される*/
	if (_object.intersects(_player)) {
		playerPos.x = playerPos.x - _tMove;
	}
	//プレイヤーのy座標＋重力＋半径でオブジェクトのy座標に当たるかどうか
	if (_player.y + _gravity + 20 >= _objectsub.y && _player.x <= _objectsub.x + 90 && _player.x >= _objectsub.x && _objectsub.y + 100 >= playerPos.y) {
		playerPos.y = _objectsub.y - 20;
		Reset(_velocity, _gravity, _jumptmp, _jumpcount);
	}//オブジェから離れたら
	else if (_player.x >= _objectsub.x + 90) {
		_jumptmp = 1;
	}
}

void CollisionOSky(double _tMove, double& _velocity, double& _gravity, int& _jumptmp, int& _jumpcount,
					Circle _player, RectF _object, RectF _objectsub/*, RectF _objectunder*/){

	/*if (_objectunder.intersects(_player)) {
		playerPos.x = playerPos.x - _tMove;
	}*/
	if (_object.intersects(_player)) {
		playerPos.x = playerPos.x - _tMove;
	}
	if (_player.y + _gravity + 20 >= _objectsub.y && _player.x <= _objectsub.x + 90 && _player.x >= _objectsub.x &&
			_objectsub.y + 100 >= playerPos.y) {
		playerPos.y = _objectsub.y - 20;
		Reset(_velocity, _gravity, _jumptmp, _jumpcount);
	}
	if (_player.x >= _objectsub.x + 90) {
		_jumptmp = 1;
	}

	//プレイやーのy座標-初速-半径がオブジェクトの一番下に当たるなら
	if (_player.y - _velocity - 20 <= _objectsub.y + 100 && _player.x <= _objectsub.x + 90 && _player.x >= _objectsub.x &&
			_objectsub.y <= playerPos.y) {
		playerPos.y = _objectsub.y + 120;
		_velocity = 0;
		_gravity = 0;
	}

	/*if (_objectunder.intersects(_player)) {
		playerPos.y = _objectsub.y + 100;
		Reset(_velocity, _gravity, _jumptmp, _jumpcount);
	}*/
}

void CollisionE(int &_die, Circle _player, Triangle _enemy){

	if (_enemy.intersects(_player)) {
		_die += 1;
	}
}


void Pattern0(double _move2, double _move3, double _bottomO, double _bottomE, double _scaleE, Circle _player,
				int &_die, double _tMove, Texture _mob, Texture _skymob,
				double &_velocity, double &_gravity, int &_jumptmp, int &_jumpcount)
{

	Triangle enemyS{ _move2 + 300, _bottomE - 300, _scaleE };
	_skymob.scaled(0.2).drawAt(_move2 + 300, _bottomE - 300);

	Triangle enemyS2{ _move2 + 300, _bottomE - 200, _scaleE };
	_skymob.scaled(0.2).drawAt(_move2 + 300, _bottomE - 200);
	
	Triangle enemy3{ _move2 + 500, _bottomE, _scaleE };
	_mob.scaled(0.3).drawAt(_move2 + 500, _bottomE);

	Triangle enemy4{ _move2 + 600, _bottomE, _scaleE };
	_mob.scaled(0.3).drawAt(_move2 + 600, _bottomE);

	Triangle enemy5{ _move3 + 300, _bottomE, _scaleE };
	_mob.scaled(0.3).drawAt(_move3 + 300, _bottomE);

	Triangle enemy6{ _move3 + 500, _bottomE, _scaleE };
	_mob.scaled(0.3).drawAt(_move3 + 500 , _bottomE);

	RectF object{ _move2, _bottomO, 1, 100 };
	object.draw(Palette::Black);

	RectF objectsub{ _move2 + 1, _bottomO, 99, 100 };
	objectsub.draw(Palette::Black);

	RectF object2{ _move2 + 650, _bottomO, 1, 100 };
	object2.draw(Palette::Black);

	//上の壁のオブジェクト
	RectF object2sub{ _move2 + 651, _bottomO, 99, 100 };
	object2sub.draw(Palette::Black);

	RectF object3{ _move3 + 100, _bottomO - 55, 1, 110 };
	object3.draw(Palette::Black);

	//上の壁のオブジェクト
	RectF object3sub{ _move3 + 101, _bottomO - 55, 99, 110 };
	object3sub.draw(Palette::Black);

	RectF object4{ _move3 + 200, _bottomO - 55, 100, 100 };
	object4.draw(Palette::Black);

	RectF object5{ _move3 + 200, _bottomO - 155, 1, 100 };
	object5.draw(Palette::Black);

	//上の壁のオブジェクト
	RectF object5sub{ _move3 + 201, _bottomO - 155, 99 , 100 };
	object5sub.draw(Palette::Black);

	RectF object6{ _move3 + 300, _bottomO - 255, 1, 100 };
	object6.draw(Palette::Black);

	//上の壁のオブジェクト
	RectF object6sub{ _move3 + 301, _bottomO - 255, 99, 100 };
	object6sub.draw(Palette::Black);

	RectF object7{ _move3 + 300, _bottomO - 155, 100, 100 };
	object7.draw(Palette::Black);

	RectF object8{ _move3 + 300, _bottomO - 55, 100, 100 };
	object8.draw(Palette::Black);

	RectF object9{ _move3 + 700, _bottomO, 1, 100 };
	object9.draw(Palette::Black);

	//上の壁のオブジェクト
	RectF object9sub{ _move3 + 701, _bottomO, 99, 100 };
	object9sub.draw(Palette::Black);

	RectF object10{ _move3 + 110, _bottomO - 45, 290, 100 };
	object10.draw(Palette::Black);

	//当たり判定
	//敵に当たったら、画面が止まる
	CollisionE(_die, _player, enemyS);
	CollisionE(_die, _player, enemyS2);
	CollisionE(_die, _player, enemy3);
	CollisionE(_die, _player, enemy4);
	CollisionE(_die, _player, enemy5);
	CollisionE(_die, _player, enemy6);

	/*オブジェクトに当たったら、押し戻される*/
	CollisionO(_tMove, _velocity, _gravity, _jumptmp, _jumpcount, _player, object, objectsub);
	CollisionO(_tMove, _velocity, _gravity, _jumptmp, _jumpcount, _player, object2, object2sub);
	CollisionO(_tMove, _velocity, _gravity, _jumptmp, _jumpcount, _player, object3, object3sub);
	CollisionO(_tMove, _velocity, _gravity, _jumptmp, _jumpcount, _player, object5, object5sub);
	CollisionO(_tMove, _velocity, _gravity, _jumptmp, _jumpcount, _player, object6, object6sub);
	CollisionO(_tMove, _velocity, _gravity, _jumptmp, _jumpcount, _player, object9, object9sub);

	if (object10.intersects(_player)) {
		playerPos.y = playerPos.y + 100;
			Reset(_velocity, _gravity, _jumptmp, _jumpcount);
	}
}

void Pattern1(double _move2, double _move3, double _bottomO, double _bottomE, double _scaleE, Circle _player,int &_die, double _tMove,
				Texture _mob, Texture _skymob, double &_velocity, double &_gravity, int &_jumptmp, int &_jumpcount){

	Triangle enemyS{ _move2 + 300, _bottomE - 300, _scaleE };
	Triangle enemyS2{ _move3 + 750, _bottomE - 200, _scaleE };
	Triangle enemy3{ _move2 + 500, _bottomE - 150, _scaleE };
	Triangle enemy4{ _move2 + 600, _bottomE - 100, _scaleE };
	Triangle enemy5{ _move3 + 300, _bottomE - 200, _scaleE };
	Triangle enemy6{ _move3 + 500, _bottomE - 100, _scaleE };
	Triangle enemy7{ _move3 + 750, _bottomE - 400, _scaleE };	
	RectF object{ _move2, _bottomO, 1, 100 };
	RectF objectsub{ _move2 + 1, _bottomO, 99, 100 };
	RectF object2{ _move2 + 400, _bottomO, 1, 100 };
	//上の壁のオブジェクト
	RectF object2sub{ _move2 + 401, _bottomO, 99, 100 };
	RectF object3{ _move3 + 100, _bottomO, 3, 100 };
	//上の壁のオブジェクト
	RectF object3sub{ _move3 + 101, _bottomO, 299, 100 };
	RectF object9{ _move3 + 700, _bottomO, 1, 100 };
	//上の壁のオブジェクト
	RectF object9sub{ _move3 + 701, _bottomO, 99, 100 };

	Pattern1Draw(_skymob, _move2, _move3, _bottomE, object, objectsub, object2, object2sub, object3, object3sub, object9, object9sub);

	//当たり判定
	//敵に当たったら、画面が止まる
	CollisionE(_die, _player, enemyS);
	CollisionE(_die, _player, enemyS2);
	CollisionE(_die, _player, enemy3);
	CollisionE(_die, _player, enemy4);
	CollisionE(_die, _player, enemy5);
	CollisionE(_die, _player, enemy6);
	CollisionE(_die, _player, enemy7);

	/*オブジェクトに当たったら、押し戻される*/
	CollisionO(_tMove, _velocity, _gravity, _jumptmp, _jumpcount, _player, object, objectsub);
	CollisionO(_tMove, _velocity, _gravity, _jumptmp, _jumpcount, _player, object2, object2sub);
	CollisionO(_tMove, _velocity, _gravity, _jumptmp, _jumpcount, _player, object9, object9sub);

	//obj3だけ長いから別処理
	if (object3.intersects(_player)) {
		playerPos.x = playerPos.x - _tMove;
	}
	if (playerPos.y + _gravity + 20 >= object3sub.y && playerPos.x <= object3sub.x + 290 && playerPos.x >= object3sub.x
			&& object3sub.y + 45 >= playerPos.y) {
		playerPos.y = 380;
		Reset(_velocity, _gravity, _jumptmp, _jumpcount);
	}
	else if (playerPos.x >= object3sub.x + 90) {
		_jumptmp = 1;
	}

}

void Pattern1Draw(Texture _skymob, double _move2, double _move3, double _bottomE, RectF _object, RectF _objectsub, RectF _object2, RectF _object2sub, RectF _object3, RectF _object3sub, RectF _object9, RectF _object9sub){

	_skymob.scaled(0.2).drawAt(_move2 + 300, _bottomE - 300);//enemyS
	_skymob.scaled(0.2).drawAt(_move3 + 750, _bottomE - 200);//enemyS2
	_skymob.scaled(0.2).drawAt(_move2 + 500, _bottomE - 150);//enemy3
	_skymob.scaled(0.2).drawAt(_move2 + 600, _bottomE - 100);//enemy4
	_skymob.scaled(0.2).drawAt(_move3 + 300, _bottomE - 200);//enemy5
	_skymob.scaled(0.2).drawAt(_move3 + 500, _bottomE - 100);//enemy6
	_skymob.scaled(0.2).drawAt(_move3 + 750, _bottomE - 400);//enemy7
	_object.draw(Palette::Black);
	_objectsub.draw(Palette::Black);
	_object2.draw(Palette::Black);
	_object2sub.draw(Palette::Black);
	_object3.draw(Palette::Black);
	_object3sub.draw(Palette::Black);
	_object9.draw(Palette::Black);
	_object9sub.draw(Palette::Black);
}

void Pattern2(double _move2, double _move3, double _bottomO, double _bottomE, double _scaleE, Circle _player, int& _die,
				double _tMove, Texture _mob, Texture _skymob, double& _velocity, double& _gravity, int& _jumptmp, int& _jumpcount){

	Triangle enemyS{ _move2 + 200, _bottomE, _scaleE };
	Triangle enemyS2{ _move2 + 300, _bottomE, _scaleE };
	Triangle enemy3{ _move2 + 400, _bottomE, _scaleE };
	Triangle enemy4{ _move2 + 500, _bottomE, _scaleE };
	Triangle enemy5{ _move2 + 700, _bottomE, _scaleE };
	Triangle enemy6{ _move3 + 200, _bottomE, _scaleE };
	Triangle enemy7{ _move3 + 350, _bottomE, _scaleE };
	Triangle enemy8{ _move3 + 550, _bottomE, _scaleE };
	RectF object{ _move2, _bottomO, 1, 100 };
	RectF objectsub{ _move2 + 1, _bottomO, 99, 100 };
	RectF object2{ _move2 + 200, _bottomO - 150, 1, 100 };
	//上の壁のオブジェクト
	RectF object2sub{ _move2 + 201, _bottomO - 150, 99, 100 };
	RectF object3{ _move2 + 400, _bottomO - 300, 1, 100 };
	//上の壁のオブジェクト
	RectF object3sub{ _move2 + 401, _bottomO - 300, 99, 100 };
	RectF object4{ _move2 + 700, _bottomO - 200, 1, 100 };
	//上の壁のオブジェクト
	RectF object4sub{ _move2 + 701, _bottomO - 200, 99, 100 };
	RectF object5{ _move3 + 300, _bottomO - 200, 1, 100 };
	//上の壁のオブジェクト
	RectF object5sub{ _move3 + 301, _bottomO - 200, 99, 100 };

	Pattern2Draw(_mob, _move2, _move3, _bottomE, object, objectsub, object2, object2sub, object3, object3sub, object4, object4sub, object5, object5sub);

	//当たり判定
	//敵に当たったら、画面が止まる
	CollisionE(_die, _player, enemyS);
	CollisionE(_die, _player, enemyS2);
	CollisionE(_die, _player, enemy3);
	CollisionE(_die, _player, enemy4);
	CollisionE(_die, _player, enemy5);
	CollisionE(_die, _player, enemy6);
	CollisionE(_die, _player, enemy7);
	CollisionE(_die, _player, enemy8);

	/*オブジェクトに当たったら、押し戻される*/
	CollisionO(_tMove, _velocity, _gravity, _jumptmp, _jumpcount, _player, object, objectsub);
	CollisionOSky(_tMove, _velocity, _gravity, _jumptmp, _jumpcount, _player, object2, object2sub);
	CollisionOSky(_tMove, _velocity, _gravity, _jumptmp, _jumpcount, _player, object3, object3sub);
	CollisionOSky(_tMove, _velocity, _gravity, _jumptmp, _jumpcount, _player, object4, object4sub);
	CollisionOSky(_tMove, _velocity, _gravity, _jumptmp, _jumpcount, _player, object5, object5sub);

}

void Pattern2Draw(Texture _mob, double _move2, double _move3, double _bottomE, RectF _object, RectF _objectsub, RectF _object2, RectF _object2sub,
					RectF _object3, RectF _object3sub, RectF _object4, RectF _object4sub, RectF _object5, RectF _object5sub){

	_mob.scaled(0.3).drawAt(_move2 + 200, _bottomE);//enemyS
	_mob.scaled(0.3).drawAt(_move2 + 300, _bottomE);//enemyS2
	_mob.scaled(0.3).drawAt(_move2 + 400, _bottomE);//enemy3
	_mob.scaled(0.3).drawAt(_move2 + 500, _bottomE);//enemy4
	_mob.scaled(0.3).drawAt(_move2 + 700, _bottomE);//enemy5
	_mob.scaled(0.3).drawAt(_move3 + 200, _bottomE);//enemy6
	_mob.scaled(0.3).drawAt(_move3 + 350, _bottomE);//enemy7
	_mob.scaled(0.3).drawAt(_move3 + 550, _bottomE);//enemy8
	_object.draw(Palette::Black);
	_objectsub.draw(Palette::Black);
	_object2.draw(Palette::Black);
	_object2sub.draw(Palette::Black);
	_object3.draw(Palette::Black);
	_object3sub.draw(Palette::Black);
	_object4.draw(Palette::Black);
	_object4sub.draw(Palette::Black);
	_object5.draw(Palette::Black);
	_object5sub.draw(Palette::Black);
}
