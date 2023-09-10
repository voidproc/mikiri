# include <Siv3D.hpp> // OpenSiv3D v0.6.11

struct CharacterFragment
{
	Vec2 pos;
	double r;
	Color color;
	double angle;
	double speed;
	int motionType;

	Vec2 getPos(double t) const
	{
		if (motionType <= 2)
		{
			return pos + Circular{ speed* t, angle } + Circular{ speed / 5.0 * Periodic::Sine1_1(3s, t), angle + 90_deg };
		}

		return pos + Circular{ speed* t, angle };
	}
};

void CharacterImage(Image& image, StringView ch)
{
	RenderTexture renderTexture{ 200, 150, ColorF{ 0 } };

	// 文字を描画
	{
		const ScopedRenderTarget2D target(renderTexture);

		FontAsset(U"ch")(ch).drawAt(120, renderTexture.size() / 2, ColorF{ 1 });
	}

	Graphics2D::Flush();

	renderTexture.readAsImage(image);
}

struct Config
{
	double density;
	double spread;
	double sizemin;
	double sizemax;
	double speedmax;

	// level: 0-1
	static Config Get(double level)
	{
		return Config{
			.density = 0.25 - 0.05 * level,
			.spread = 2.0 + 2.0 * level,
			.sizemin = 3.0,
			.sizemax = 12.0,
			.speedmax = 200.0 + 400 * level,
		};
	}
};

void GenerateFragments(Array<CharacterFragment>& fragments, Image& answerImage, StringView ch, double level)
{
	answerImage.clear();

	CharacterImage(answerImage, ch);

	fragments.clear();

	auto config = Config::Get(level);

	// 文字の形に沿って並べる

	for (auto iy : step(answerImage.height()))
	{
		for (auto ix : step(answerImage.width()))
		{
			if (answerImage.getPixel(ix, iy, ImageAddressMode::BorderBlack).grayscale() > 0.5)
			{
				if (RandomBool(config.density))
				{
					fragments.push_back({
							.pos = Vec2{ ix, iy } + Circular{ Random()* config.spread, Random()* Math::TwoPi },
							.r = config.sizemin + config.sizemax * Random(),
							.color = Color(1, 0.1 + Random() * 0.2),
							.angle = Random() * Math::TwoPi,
							.speed = 20 + Random() * config.speedmax,
							.motionType = Random<int>(0, 5),
						});
				}
			}
		}
	}

	// ランダムに配置する

	for (int i : step(600))
	{
		fragments.push_back({
				.pos = answerImage.size() / 2.0 + Circular{ Random()* answerImage.width() * 1.5, Random()* Math::TwoPi },
				.r = config.sizemin + config.sizemax * Random(),
				.color = Color(1, 0.1 + Random() * 0.2),
				.angle = Random() * Math::TwoPi,
				.speed = 20 + Random() * config.speedmax / 2,
				.motionType = Random<int>(0, 5),
			});
	}

	// 位置をシーンサイズに合わせる

	const Vec2 scale = Scene::Size() * 1.0 / answerImage.size();

	for (auto& f : fragments)
	{
		f.pos *= scale;
		f.pos.y -= 56;
	}
}

String RandomKanji(int strokesMax = 4)
{
	constexpr std::array<StringView, 30> kanji = {
		U""_sv,
		U""_sv,
		U""_sv,
		U""_sv,
		U"円王火月犬手水中天日文木六五引牛元戸午公今止少心切太内父分方毛友化区反予欠氏不夫支比仏尺収仁片牙介刈凶斤幻互勾孔升冗井双丹弔爪斗屯匂匹乏厄"_sv,
		U"右玉左四出正生石田白本目立外兄古広市矢台冬半母北用央去号皿仕写主申世他打代皮氷平由礼以加功札史司失必付辺包末未民令圧永可刊旧句示犯布弁穴冊処庁幼凹瓦且甘丘巨玄巧甲込𠮟囚汁召尻斥仙占旦奴凸丼尼氾払丙矛"_sv,
		U"気休糸字耳先早竹虫年百名羽回会交光考行合寺自色西多地池当同肉米毎安曲血向死次式守州全有羊両列衣印各共好成争仲兆伝灯老因仮件再在舌団任宇灰危机吸后至存宅扱芋汚汗缶企伎吉朽臼叫仰刑江旨芝朱舟充旬巡如匠尽迅壮吐弐肌伐帆汎妃伏忙朴妄吏劣"_sv,
		U"花貝見車赤足村男町何角汽近形言谷作社図声走体弟売麦来里医究局君決住助身対投豆坂返役位囲改完希求芸告材児初臣折束低努兵別利良冷労応技均災志似序条状判防余快我系孝困私否批忘乱卵亜壱戒肝含岐忌却狂吟串迎呉坑抗攻更克佐沙伺寿秀床抄肖伸芯辛吹杉即汰妥択沢但沖沈呈廷那尿妊忍把伯抜伴阪尾肘扶芳邦坊妨没妙冶妖抑沃励戻呂弄"_sv,
		U"雨学金空青林画岩京国姉知長直店東歩妹明門夜委育泳岸苦具幸使始事実者取受所昔注定波板表服物放味命油和英果芽官季泣協径固刷参治周松卒底的典毒念府法牧例易往価河居券効妻枝舎述招承制性版肥非武延沿拡供呼刻若宗垂担宙忠届乳拝並宝枚宛依炎押旺欧殴岡佳苛怪拐劾岳玩奇祈宜拒拠享況屈茎肩弦股虎拘肯昆采刹刺祉肢侍邪呪叔尚昇沼炊枢姓征斉析拙狙阻卓拓抽坪抵邸泥迭妬到突奈杯拍泊迫彼披泌苗怖阜附侮沸併奉抱泡房肪奔枕抹岬免茂盲弥拉炉枠"_sv,
		U"音草科海活計後思室首秋春食星前茶昼点南風屋界客急級係研県指持拾重昭乗神相送待炭柱追度畑発美秒品負面洋胃栄紀軍型建昨祝信省浅単飛変便約勇要逆限故厚査政祖則退独保迷映革巻看皇紅砂姿城宣専泉洗染奏段派背肺律哀威為畏茨咽姻疫怨卸架悔皆垣柿括冠軌虐糾峡挟狭契孤弧枯侯恒洪荒郊香拷恨砕削柵咲拶施狩臭柔俊盾叙浄拭侵津甚帥是牲窃荘促俗耐怠胎胆挑勅珍亭貞帝訂怒逃洞峠栃虹卑眉訃赴封柄胞某冒勃盆昧幽柳侶厘郎"_sv,

		U"校夏家記帰原高紙時弱書通馬員院荷起宮庫根酒消真息速庭島配倍病勉流旅案害挙訓郡候航差殺残借笑席倉孫帯徒特梅粉脈浴料連益桜恩格個耕財師修素造能破俵容留株胸降骨座蚕射従純除将針値展討党納俳班秘陛朗挨唄畝悦宴翁俺華蚊核釜陥既飢鬼恐恭脅恵桁倹兼剣拳軒娯悟貢剛唆挫宰栽剤索桟恣脂疾酌殊珠袖准殉徐宵症祥称辱唇娠振浸陣粋衰凄逝脊隻扇栓租捜挿桑捉泰託恥致畜逐秩衷酎朕逓哲途倒凍唐桃透胴匿悩剝畔般疲被姫浜敏浮紛哺捕浦俸倣峰砲剖紡埋眠娘冥耗紋竜倫涙烈恋浪脇捗"_sv,
		U"魚強教黄黒細週雪船組鳥野理悪球祭終習宿商章深進族第帳笛転都動部問貨械救健康菜産唱清巣側停堂得敗票副望陸移液眼基寄規許経険現混採授術常情責接設率断張貧婦務略異域郷済視捨推盛窓探著頂脳閉訪密訳郵欲翌尉萎逸淫陰菓崖涯殻郭掛喝渇乾勘患貫亀偽菊脚虚菌惧偶掘啓掲渓蛍舷控梗頃婚痕紺彩斎埼崎惨斬鹿執赦斜蛇釈寂羞渋淑粛庶渉紹訟剰紳酔崇据惜戚旋措粗掃曹曽爽唾堆袋逮脱淡窒彫眺釣陳偵添悼盗陶豚貪梨軟捻粘婆排培陪舶販描猫瓶符偏崩堀麻猛唯悠庸粒隆涼猟陵累"_sv,
		U"森雲絵間場晴朝答道買番飲運温階寒期軽湖港歯集暑勝植短着湯登等童悲筆遊葉陽落開覚喜給極景結最散順焼象然隊達貯博飯費満無量街営過賀検減証税絶測属貸提程統備評富復報貿割貴勤筋敬裁策詞就衆善創装尊痛晩補棒揮握嵐偉椅詠越媛援奥渦葛喚堪換敢棺款閑幾棋欺喫距御暁琴遇隅圏堅雇喉慌硬絞項詐酢傘紫滋軸湿煮循掌晶焦硝粧詔畳殖診尋須遂随婿疎訴喪痩葬堕惰替棚弾遅貼超椎塚堤渡塔搭棟痘筒鈍廃媒斑蛮扉普幅雰塀遍募傍帽喩愉湧猶裕雄揚揺絡痢硫塁裂廊惑湾腕"_sv,
		U"園遠楽新数電話暗意感漢業詩想鉄農福路愛塩試辞照節戦続置腸働解幹義禁群鉱罪資飼準勢損墓豊夢預絹源署傷蒸聖誠暖賃腹幕盟裏彙違煙猿鉛虞嫁暇禍靴雅塊楷慨蓋該較隔滑褐勧寛頑棄毀詰嗅僅愚窟傾携継詣隙傑嫌献遣誇鼓碁溝傲債催塞歳載搾嗣慈嫉腫愁酬奨詳飾触寝慎腎睡裾跡摂煎羨腺詮践禅塑僧賊滞滝嘆痴稚蓄跳艇溺塡殿塗督頓漠鉢搬煩頒微蜂飽睦滅誉溶腰裸雷酪慄虜鈴零廉賂楼賄"_sv,
		U"歌語算読聞鳴駅銀鼻様緑練管関旗漁察種静説歴演慣境構際雑酸精製銭総像増態適銅徳複綿領閣疑誤穀誌磁障層認暮模維隠寡箇概駆熊綱酵豪酷獄魂雌漆遮需銃塾緒彰誓箋漸遭憎駄奪端綻嫡徴漬摘滴稲寧髪罰閥碑漂腐蔑慕貌僕墨膜慢漫蜜銘網誘瘍踊辣僚瑠暦漏遡遜"_sv,
		U"線横談調箱億課器賞選熱標養輪確潔賛質敵導編暴遺劇権熟諸蔵誕潮論慰影鋭謁閲縁稼餓潰潟歓監緩畿輝儀戯窮緊勲慶憬稽撃稿駒撮暫摯賜趣潤遵憧衝縄嘱審震穂請潜遷槽踪諾誰鋳駐嘲澄墜締徹撤踏罵輩賠箸範盤罷膝賓敷膚賦舞噴墳憤幣弊蔽餅舗褒撲摩魅黙憂窯履璃慮寮霊餌"_sv,
		U"親頭館橋整薬機積録衛興築燃輸激憲鋼樹縦操糖奮緯憶穏壊懐諧骸獲憾還凝錦薫憩賢錮衡墾錯諮儒獣壌嬢錠薪醒薦膳濁壇緻諦賭篤曇濃薄縛繁避壁縫膨謀頰磨麺諭融擁謡頼隣隷錬"_sv,
		U"講謝績厳縮優覧曖臆嚇轄環擬犠矯謹謙鍵購懇擦爵醜償礁繊鮮燥霜戴濯鍛聴謄瞳謎鍋頻闇翼療瞭齢"_sv,
		U"顔曜題観験類額織職簡難臨穫顎鎌韓騎襟繭顕鎖瞬繕礎騒贈懲鎮藤闘藩覆璧癖翻癒濫藍糧"_sv,
		U"願鏡識警臓韻艶繰鶏鯨璽蹴髄瀬藻覇爆譜簿霧羅離麗麓"_sv,

		U"議競護響懸鐘譲醸籍騰欄艦顧鶴魔躍露驚襲籠鑑鬱"_sv,
		U""_sv,
		U""_sv,
		U""_sv,
		U""_sv,
		U""_sv,
		U""_sv,
		U""_sv,
		U""_sv,
		U""_sv,
	};

	String s{};

	for (int i = strokesMax - 4; i <= strokesMax; ++i)
	{
		s += kanji[i];
	}

	return s.substr(Random<int>(0, s.length() - 1), 1);
}

struct Question
{
	String ch;
	double start;

	static Question Get(double level)
	{
		return Question{
			.ch = RandomKanji(4 + 16 * level),
			.start = -(2.5 + 2.5 * Random()),
		};
	}
};

void DrawFragments(const Array<CharacterFragment>& fragments, double timeSec)
{
	for (const auto& f : fragments)
	{
		Circle{ f.getPos(timeSec), f.r }.draw(ColorF{ 1, 0.5 });
	}
}

void DrawInputArea(StringView answer)
{
	const auto inputRect = RectF{ 320, 64 }.setCenter(Scene::Rect().bottomCenter().movedBy(0, -32 - 12));
	RectF{ Scene::Width(), 64 + 32 * 2 }.setCenter(inputRect.center()).draw(ColorF{ 0, 0.8 });
	inputRect.drawFrame(2.0, Palette::Silver);

	const auto& font = FontAsset(U"sys");
	constexpr double FontSize = 42.0;
	const double FontScale = FontSize / font.fontSize();
	const auto textRegion = font(answer).regionAt(FontSize, inputRect.center());
	auto penPos = inputRect.center().movedBy(-textRegion.w / 2, 0);

	Line cursor{ penPos.movedBy(0, -20), penPos.movedBy(0, 20) };

	{
		const ScopedCustomShader2D shader{ Font::GetPixelShader(font.method()) };

		for (auto [index, glyph] : Indexed(font.getGlyphs(answer)))
		{
			penPos.x += glyph.xAdvance / 2 * FontScale;
			glyph.texture.scaled(FontScale).drawAt(penPos);
			penPos.x += glyph.xAdvance / 2 * FontScale;

			// カーソル
			cursor = Line{ penPos.movedBy(0, -20), penPos.movedBy(0, 20) };
		}

		for (auto [index, glyph] : Indexed(font.getGlyphs(TextInput::GetEditingText())))
		{
			penPos.x += glyph.xAdvance / 2 * FontScale;
			glyph.texture.scaled(FontScale).drawAt(penPos, Palette::Gray);
			penPos.x += glyph.xAdvance / 2 * FontScale;
		}
	}

	// カーソル描画
	cursor.draw(2, ColorF{ 1, 0.1 + Periodic::Jump0_1(0.5s) });

	FontAsset(U"sys")(U"出てくる漢字１文字を答えてください").drawBaseAt(16, inputRect.topCenter().movedBy(0, -12));
}

class BloomEffect
{
public:
	BloomEffect(const Size& size)
		: size_{ size }, blur1_{ size }, internal1_{ size }, blur4_{ size / 4 }, internal4_{ size / 4 }, blur8_{ size / 8 }, internal8_{ size / 8 }
	{

	}

	const ScopedRenderTarget2D getRenderTarget() const
	{
		return ScopedRenderTarget2D{ blur1_.clear(ColorF{ 0 }) };
	}

	void update()
	{
		Shader::GaussianBlur(blur1_, internal1_, blur1_);

		Shader::Downsample(blur1_, blur4_);
		Shader::GaussianBlur(blur4_, internal4_, blur4_);

		Shader::Downsample(blur4_, blur8_);
		Shader::GaussianBlur(blur8_, internal8_, blur8_);
	}

	void draw(const Vec2& pos = { 0, 0 }, const Color& color = Palette::White, double intensity1 = 0.4, double intensity4 = 0.4, double intensity8 = 0.4) const
	{
		const ScopedRenderStates2D blend{ BlendState::Additive };

		blur1_.draw(pos, ColorF{ color, color.a * intensity1 / 255 });
		blur4_.resized(size_).draw(pos, ColorF{ color, color.a * intensity4 / 255 });
		blur8_.resized(size_).draw(pos, ColorF{ color, color.a * intensity8 / 255 });
	}

private:
	const Size size_;
	const RenderTexture blur1_;
	const RenderTexture internal1_;
	const RenderTexture blur4_;
	const RenderTexture internal4_;
	const RenderTexture blur8_;
	const RenderTexture internal8_;
};

void DrawBloom(BloomEffect& bloom)
{
	double a1 = 0.2 + 0.09 * (Periodic::Sine0_1(29ms) * Periodic::Sine0_1(72ms));
	double a4 = 0.4 + 0.08 * (Periodic::Sine0_1(58ms) * Periodic::Sine0_1(92ms));
	double a8 = 0.7 + 0.05 * (Periodic::Sine0_1(72ms) * Periodic::Sine0_1(80ms));
	bloom.draw({ 0, 0 }, Palette::White, a1, a4, a8);
}

void Main()
{
	Scene::SetBackground(ColorF{ 0 });

	FontAsset::Register(U"title", FontMethod::MSDF, 80, Typeface::Thin);
	FontAsset::Register(U"ch", FontMethod::MSDF, 48, Typeface::Light);
	FontAsset::Register(U"sys", FontMethod::MSDF, 48, Typeface::Bold);

	System::SetTerminationTriggers(UserAction::CloseButtonClicked);

	Array<CharacterFragment> fragments;
	Image answerImage;
	Texture answerTexture;

	double level = 0;
	int stage = 1;
	int correct = 0;
	int replay = 3;
	auto q = Question::Get(level);
	String answer{};

	Stopwatch timeTitle{ StartImmediately::Yes };
	Stopwatch timeQuestionNumber;
	Stopwatch time{ SecondsF(q.start) };
	Stopwatch timeJudge{};
	Stopwatch timeAnswer{};
	Stopwatch timeGameOver{};

	String questions = q.ch;

	GenerateFragments(fragments, answerImage, q.ch, level);
	answerTexture = Texture{ answerImage };

	BloomEffect bloom{ Scene::Size() };


	while (System::Update())
	{
		if (timeTitle.isRunning())
		{
			if (timeTitle > 1.0s && KeyEnter.down())
			{
				timeTitle.reset();
				timeQuestionNumber.restart();
			}


			// 描画

			const auto font = FontAsset(U"title");
			const StringView title = U"瞬字の見切り"_sv;
			auto titleRect = font(title).region().scaled(1, 1.2).setCenter(Scene::CenterF().movedBy(0, 80));
			auto penPos = titleRect.pos;

			{
				const ScopedCustomShader2D shader{ Font::GetPixelShader(font.method()) };

				for (auto [index, glyph] : Indexed(font.getGlyphs(title)))
				{
					penPos.x += glyph.xAdvance * 0.5 / 2;
					glyph.texture.scaled(0.75, 1.2).draw(Arg::bottomLeft = penPos + Vec2{2, 0}, ColorF{ Palette::Cyan, 0.5 });
					glyph.texture.scaled(0.75, 1.2).draw(Arg::bottomLeft = penPos + Vec2{-2, 0}, ColorF{ Palette::Magenta, 0.5 });
					glyph.texture.scaled(0.75, 1.2).draw(Arg::bottomLeft = penPos);
					penPos.x += glyph.xAdvance * (1 - 0.5 / 2);
				}
			}

			if (timeTitle > 1.0s)
			{
				FontAsset(U"sys")(U"Enterではじめる").drawAt(20, Scene::CenterF().movedBy(0, 96), ColorF{ 0.8, Periodic::Jump0_1(0.8s) });
			}
		}

		if (timeQuestionNumber.isRunning())
		{
			// (1)「第〇問」を表示中

			if (timeQuestionNumber > 1.5s)
			{
				timeQuestionNumber.reset();
				time.set(SecondsF(q.start));
				time.start();
			}

			// 描画

			Scene::Rect().draw(Color{ 0 });
			FontAsset(U"sys")(U"第 {} 問"_fmt(stage)).drawAt(80, Scene::CenterF());
		}

		if (time.isRunning())
		{
			// (2) 文字が表示される／回答する

			// 残り時間
			constexpr double RemainTimeMax = 13.0;
			const double timeRemain = Max(RemainTimeMax - time.sF(), 0.0);

			// 残り時間がゼロになるかEnterが押されたら判定へ

			if (not timeJudge.isRunning())
			{
				if (timeRemain < 1e-3 || (answer && TextInput::GetEditingText().empty() && KeyEnter.down()))
				{
					//time.pause();
					timeJudge.restart();
				}

				// 回答の入力
#if SIV3D_PLATFORM(WEB)
				Platform::Web::TextInput::SetFocusToTextInput(true);
#endif
				TextInput::UpdateText(answer, TextInputMode::AllowBackSpaceDelete);
			}


			// 描画

			// 文字
			DrawFragments(fragments, time.sF());

			{
				const auto renderTarget = bloom.getRenderTarget();
				DrawFragments(fragments, time.sF());
			}

			bloom.update();

			DrawBloom(bloom);

			// 回答
			DrawInputArea(answer);

			// 第〇問（左上）
			FontAsset(U"sys")(U"第 {} 問"_fmt(stage)).draw(20, Vec2{ 4, 0 });

			// 残り時間が10秒を切ったらカウントダウンを表示
			if (timeRemain < 10 && not timeJudge.isRunning())
			{
				FontAsset(U"sys")(U"{}"_fmt(Max(1, (int)timeRemain + 1)))
					.drawAt(TextStyle::Outline(0.4, ColorF{ Palette::Darkgray, 0.3 }), 180 + 20 * (10 - timeRemain), Scene::CenterF());
			}
		}

		if (timeJudge.isRunning())
		{
			const bool isCorrect = q.ch == answer;

			// 答えの表示へ
			if (timeJudge > 1.5s)
			{
				time.reset();
				timeJudge.reset();
				timeAnswer.restart();

				if (isCorrect) ++correct;
			}

			const double t0_1 = Clamp(timeJudge.sF() / 0.5, 0.0, 1.0);
			const double t1_0 = EaseInExpo(Clamp(1.0 - timeJudge.sF() / 0.5, 0.0, 1.0));

			if (timeJudge < 1.0s)
			{
				if (isCorrect)
				{
					Circle{ Scene::CenterF(), 200 + 500 * t1_0 }
					.drawFrame(70, ColorF{ Palette::Lawngreen, (0.3 + 0.5 * Periodic::Square0_1(0.1s)) * t0_1 });
				}
				else
				{
					Shape2D::Cross(300, 80, Scene::CenterF())
						.asPolygon()
						.scaledAt(Scene::CenterF(), 1 + 2 * t1_0)
						.draw(ColorF{ Palette::Firebrick, (0.3 + 0.5 * Periodic::Square0_1(0.1s)) * t0_1 });
				}
			}
		}

		if (timeAnswer.isRunning())
		{
			const bool isCorrect = q.ch == answer;

			// Enterで次へ
			if (timeAnswer > 1.0s && KeyEnter.down())
			{
				timeAnswer.reset();

				if (isCorrect)
				{
					// 次の問題へ

					level = Clamp(level + 0.1, 0.0, 1.0);
					stage += 1;
					q = Question::Get(level);
					questions += U"," + q.ch;
					answer = U"";

					GenerateFragments(fragments, answerImage, q.ch, level);
					answerTexture = Texture{ answerImage };

					timeQuestionNumber.restart();

					// すぐ下のDrawFragmentsでネタバレしないように.
					continue;
				}
				else
				{
					// ゲームオーバーへ

					timeGameOver.restart();
					continue;
				}
			}


			// 描画

			// 文字
			answerTexture.resized(Scene::Size()).drawAt(Scene::CenterF().movedBy(0, -56), ColorF{ 1, 0.5 });
			DrawFragments(fragments, 0);

			{
				const auto renderTarget = bloom.getRenderTarget();
				DrawFragments(fragments, 0);
			}

			bloom.update();

			DrawBloom(bloom);

			// 答え（下部）
			FontAsset(U"sys")(U"答えは {}"_fmt(q.ch)).drawAt(TextStyle::Shadow(Vec2{ 4, 4 }, ColorF{ 0 }), 42, Scene::Rect().bottomCenter().movedBy(0, -56));

			// Enterで次の問題 or 結果表示へ
			if (timeAnswer > 1.0s)
			{
				const auto region = FontAsset(U"sys")(isCorrect ? U"Enterで次の問題へ " : U"Enterを押してください ").draw(20, Arg::bottomRight = Scene::Rect().br());
				FontAsset(U"sys")(U"▶").draw(20, Arg::rightCenter = region.leftCenter(), ColorF{ 1, Periodic::Square0_1(0.8s) });
			}


			// フェード
			Scene::Rect().draw(ColorF{ 0, 1.0 - Clamp(timeAnswer.sF() / 0.3, 0.0, 1.0) });
		}

		if (timeGameOver.isRunning())
		{
			// Enterで次へ
			if (timeGameOver > 1.0s && KeyEnter.down())
			{
				timeGameOver.reset();

				// 1問目へ

				level = 0;
				stage = 1;
				q = Question::Get(level);
				questions = q.ch;
				answer = U"";

				GenerateFragments(fragments, answerImage, q.ch, level);
				answerTexture = Texture{ answerImage };

				timeQuestionNumber.restart();
			}


			// 描画

			{
				const Transformer2D transform{ Mat3x2::Translate(0, -56) };

				// 連続正答数

				const auto textRect1 = FontAsset(U"sys")(U"{}"_fmt(correct)).regionBaseAt(80, Scene::CenterF().movedBy(20, -40));
				auto textRect2 = FontAsset(U"sys")(U"連続正答は ").regionBaseAt(48, Scene::CenterF().movedBy(0, -40));
				auto textRect3 = FontAsset(U"sys")(U" 回でした").regionBaseAt(48, Scene::CenterF().movedBy(0, -40));

				textRect2.x = textRect1.x - textRect2.w;
				textRect3.x = textRect1.rightX();

				const ColorF color{ 1.0, Clamp(timeGameOver.sF() / 1.5, 0.0, 1.0) };

				FontAsset(U"sys")(U"{}"_fmt(correct)).draw(80, textRect1, ColorF{ Palette::Lawngreen, color.a });
				FontAsset(U"sys")(U"連続正答は "_fmt(correct)).draw(48, textRect2, color);
				FontAsset(U"sys")(U" 回でした"_fmt(correct)).draw(48, textRect3, color);

				const auto questionsRect = RectF{ 400, 200 }.setCenter(Scene::Rect().bottomCenter().movedBy(0, -160));
				FontAsset(U"sys")(U"出題一覧: {}"_fmt(questions))
					.draw(24, questionsRect, color);
				questionsRect.stretched(4, 2).drawFrame(2.0, Palette::Gray);
			}


			// Enterではじめから
			if (timeGameOver > 1.0s)
			{
				const auto region = FontAsset(U"sys")(U"Enterではじめから ").draw(20, Arg::bottomRight = Scene::Rect().br());
				FontAsset(U"sys")(U"▶").draw(20, Arg::rightCenter = region.leftCenter(), ColorF{ 1, Periodic::Square0_1(0.8s) });
			}
		}
	}
}
