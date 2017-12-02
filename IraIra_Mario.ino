int buzzerPin = 53;     // ブザーを接続したピン番号

const int StartSwitchPin = 55; //スタート兼終了ピン
const int MistakeSwitchPin = 56; //コースにつながったピン
const int GoalSwitchPin = 57; //ゴールにつながったピン

const int LEDNum = 3;
const int LedPin[LEDNum] = {58, 59, 60};
int HitPoint = LEDNum;


const int VibrationPin = 61;

unsigned long EndTime = 60000; //開始から終了秒数*1000
unsigned long StartTime = 0; //setupからスタートした秒数*1000

boolean FirstFlg;
boolean StartFlg;
boolean GoalFlg;
boolean MistakeFlg;

const int ET[12][10]  = {
  {16, 33,  65, 131, 262, 523, 1047, 2093, 4186,  8372}, // 0 ド
  {18, 37,  73, 147, 294, 587, 1175, 2349, 4699,  9397}, // 1 レ
  {21, 41,  82, 165, 330, 659, 1319, 2637, 5274, 10548}, // 2 ミ
  {22, 44,  87, 175, 349, 698, 1397, 2794, 5578, 11175}, // 3 ファ
  {25, 49,  98, 195, 392, 784, 1568, 3136, 6271, 12543}, // 4 ソ
  {27, 55, 110, 220, 440, 880, 1760, 3520, 7040, 14080}, // 5 ラ
  {31, 62, 123, 247, 494, 988, 1976, 3951, 7902, 15804}, // 6 シ
  {17, 35,  69, 139, 277, 554, 1109, 2217, 4435,  8869}, // 7 ド＃
  {19, 39,  78, 156, 311, 622, 1245, 2489, 4978,  9956}, // 8 レ＃
  {23, 46,  93, 185, 370, 740, 1480, 2960, 5919, 11839}, // 9 ファ＃
  {26, 52, 104, 208, 415, 831, 1661, 3322, 6644, 13289}, //10 ソ＃
  {29, 58, 117, 233, 466, 932, 1865, 3729, 7458, 14917}  //11 ラ＃
};

void setup() {
  randomSeed(analogRead(7));
  Serial.begin(9600);
  pinMode(StartSwitchPin, INPUT);
  pinMode(MistakeSwitchPin, INPUT);
  pinMode(GoalSwitchPin, INPUT);
  StartFlg = false;
  GoalFlg = false;
  MistakeFlg = false;
  for (int i = 0; i < LEDNum; i++) {
    pinMode(LedPin[i], OUTPUT);
    digitalWrite(LedPin[i], HIGH);
  }
}

void loop() {
  /*
  if (digitalRead(StartSwitchPin) == HIGH) {
    while (digitalRead(StartSwitchPin) == HIGH);
    StartSwitchPinChattering();//チャタリング防止
    StartTime = millis();// 初期化
    FirstFlg = true;
    Start();
    while (StartFlg == true) {
      if (MistakeFlg == true) {   //アルミに棒が当たったら
        GameOver();
        BreakTime(2000);
        Famima();
        MistakeFlg = false;
        GoalFlg  = false;
        StartFlg = false;
      }
      else if (GoalFlg == true) { //ゴールしたら
        int a = random() % 11;
        switch (a) {
          case 0: Zen(); break;
          default: OneUp(); break;
        }
        GoalFlg = false;
        MistakeFlg = false;
        StartFlg = false;
      }
      else {
        Basement();
      }
    }
    while (digitalRead(StartSwitchPin) == HIGH);
    StartSwitchPinChattering();//チャタリング防止
    for (int i = 0; i < 3; i++) {
      digitalWrite(LedPin[i], HIGH);
    }
    HitPoint = LEDNum;
  }
  */
  GameOver();
}

void StartSwitchPinChattering() {
  int  i = 0;
  while (1) {
    if (digitalRead(StartSwitchPin) == LOW) {
      delay(1);
      i++;
      if (i == 100) break;
    }
    else i = 0;
  }
}

void FlgControl() {
  if ((FirstFlg == true || StartFlg == true ) && digitalRead(StartSwitchPin) == HIGH) { //スタートしてるときの強制終了
    StartFlg = false;
    MistakeFlg = false;
    GoalFlg = false;
    FirstFlg = false;
  }
  else if (GoalFlg == false && digitalRead(GoalSwitchPin) == HIGH) { //ゴール
    GoalFlg = true;
  }
  else if (StartFlg == true && MistakeFlg == false && GoalFlg == false &&  HitPoint > 0 && digitalRead(MistakeSwitchPin) == HIGH) {
    HitPoint--;
    digitalWrite(LedPin[HitPoint], LOW);
    if (HitPoint > 0) Dokan();
  }
  if (HitPoint == 0 || (MistakeFlg == false && (millis() - StartTime > EndTime))) {
    MistakeFlg = true;
  }
}

void Dokan() {// マリオの土管
  if (StartFlg == true && MistakeFlg == false && GoalFlg == false) {
    const int DokanNum = 11; const int QN = 150; const int oct = 5;
    int DokanDeta[DokanNum][2] = {
      {ET[3][oct], QN / 3}, {ET[0][oct], QN / 3}, {ET[3][oct - 1], QN / 3}, {0, QN / 3},
      {ET[3][oct], QN / 3}, {ET[0][oct], QN / 3}, {ET[3][oct - 1], QN / 3}, {0, QN / 3},
      {ET[3][oct], QN / 3}, {ET[0][oct], QN / 3}, {ET[3][oct - 1], QN / 3}
    };
    if (StartFlg == true) {
      digitalWrite(VibrationPin, HIGH);
      for (int i = 0; i < DokanNum; i++) { // メロディーを流す
        tone(buzzerPin, DokanDeta[i][0], DokanDeta[i][1]);
        delay(DokanDeta[i][1]);
        if (i == DokanNum / 2)digitalWrite(VibrationPin, LOW);
      }
      delay(200);
    }
  }
}

void Start() { // NHK時報
  const int StartNum = 7; const int QN = 500; const int oct = 6;
  int StartDeta[StartNum][2] = {
    {ET[5][oct - 1], QN}, {0, QN},
    {ET[5][oct - 1], QN}, {0, QN},
    {ET[5][oct - 1], QN}, {0, QN},
    {ET[5][oct], QN * 2}
  };
  for (int i = 0; i < StartNum; i++) { // メロディーを流す
    if (i == 6) {
      StartTime = millis();
      FirstFlg = false; StartFlg = true;
      tone(buzzerPin, StartDeta[i][0], StartDeta[i][1]);
      TryingBreakTime(StartDeta[i][1]);
    }
    else if (i == 0 || i == 1 || i == 2) {
      tone(buzzerPin, StartDeta[i][0], StartDeta[i][1]);
      delay(StartDeta[i][1]);
    }
    else {
      tone(buzzerPin, StartDeta[i][0], StartDeta[i][1]);
      BreakTime(StartDeta[i][1]);
      if (FirstFlg == false) break;
    }
  }
}

void Basement() { //マリオの地下エリア
  if (StartFlg == true && MistakeFlg == false && GoalFlg == false) {
    const int BasementNum = 49; const int QN = 200; const int oct = 6;
    int BasementDeta[BasementNum][2] = {
      {ET[0][oct - 1], QN}, {ET[0][oct], QN}, {ET[5][oct - 2], QN}, {ET[5][oct - 1], QN},
      {ET[11][oct - 2], QN}, {ET[11][oct - 1], QN},
      {0, QN * 6}, {ET[0][oct - 1], QN}, {ET[0][oct], QN}, {ET[5][oct - 2], QN}, {ET[5][oct - 1], QN},
      {ET[11][oct - 2], QN}, {ET[11][oct - 1], QN},
      {0, QN * 6}, {ET[3][oct - 1], QN},  {ET[3][oct], QN}, {ET[1][oct - 1], QN}, {ET[1][oct], QN},
      {ET[8][oct - 1], QN}, {ET[8][oct], QN},
      {0, QN * 6}, {ET[3][oct - 1], QN},  {ET[3][oct], QN}, {ET[1][oct - 1], QN}, {ET[1][oct], QN},
      {ET[8][oct - 1], QN}, {ET[8][oct], QN},
      {0, QN * 2}, {ET[8][oct - 1], (QN * 2) / 3}, {ET[1][oct - 1], (QN * 2) / 3}, {ET[7][oct - 1], (QN * 2) / 3},
      {ET[0][oct - 1], QN * 2}, {ET[8][oct - 1], QN * 2}, {ET[1][oct - 1], QN * 2}, {ET[10][oct - 2], QN * 2},
      {ET[4][oct - 2], QN * 2}, {ET[7][oct - 1], QN * 2}, {ET[0][oct - 1], (QN * 2) / 3},
      {ET[9][oct - 1], (QN * 2) / 3}, {ET[3][oct - 1], (QN * 2) / 3}, {ET[2][oct - 1], (QN * 2) / 3},
      {ET[11][oct - 1], (QN * 2) / 3}, {ET[5][oct - 1], (QN * 2) / 3}, {ET[10][oct - 1], (QN * 4) / 3},
      {ET[8][oct - 1], (QN * 4) / 3}, {ET[6][oct - 2], (QN * 4) / 3},
      {ET[11][oct - 2], (QN * 4) / 3}, {ET[5][oct - 2], (QN * 4) / 3}, {ET[10][oct - 2], (QN * 4) / 3}
    };
    while (StartFlg == true && MistakeFlg == false && GoalFlg == false) {
      TryingBreakTime(100);
      for (int i = 0; i < BasementNum; i++) { // メロディーを流す
        tone(buzzerPin, BasementDeta[i][0], BasementDeta[i][1]);
        if (HitPoint == 1 || millis() - StartTime > EndTime - 10000) {
          TryingBreakTime(BasementDeta[i][1] / 1.5);
        } else {
          TryingBreakTime(BasementDeta[i][1]);
        }
        if (StartFlg == false || MistakeFlg == true || GoalFlg == true) break;
      }
      if (StartFlg == false || MistakeFlg == true || GoalFlg == true) break;
    }
  }
}


void GameOver() { //マリオのゲームオーバー
  if (StartFlg == true) {
    const int GameOverNum = 16; const int QN = 200; const int oct = 6;
    int GameOverDeta[GameOverNum][2] = {
      {ET[2][oct], QN / 2}, {ET[0][oct], QN / 2}, {ET[3][oct - 1], QN / 2}, {0, QN * 2}, {ET[1][oct - 1], QN / 2},
      {ET[0][oct], QN}, {0, QN / 2}, {ET[0][oct], QN / 2}, {ET[0][oct], QN}, {ET[6][oct - 1], QN},
      {ET[5][oct - 1], QN / 2}, {ET[4][oct - 1], QN},
      {ET[1][oct - 1], QN}, {0, QN / 2}, {ET[1][oct - 2], QN / 2}, {ET[4][oct - 3], QN}
    };
    if (StartFlg == true) {
      digitalWrite(VibrationPin, HIGH);
      for (int i = 0; i < GameOverNum; i++) { // メロディーを流す
        tone(buzzerPin, GameOverDeta[i][0], GameOverDeta[i][1]);
        BreakTime(GameOverDeta[i][1]);
        if (StartFlg == false) break;
        if (i == GameOverNum / 2)digitalWrite(VibrationPin, LOW);
      }
    }
  } noTone(buzzerPin);
}

void Toccata() { //バッハ「トッカータとフ－ガ ニ短調」
  const int ToccataNum = 10; const int QN = 200; const int oct = 6;
  int ToccataDeta[ToccataNum][2] = {
    {ET[5][oct], QN}, {ET[4][oct], QN}, {ET[5][oct], QN * 6}, {0, QN * 2}, {ET[4][oct], QN},
    {ET[3][oct], QN}, {ET[2][oct], QN}, {ET[1][oct], QN}, {ET[7][oct], QN * 2}, {ET[1][oct], QN * 4}
  };
  if (StartFlg == true) {
    for (int i = 0; i < ToccataNum; i++) { // メロディーを流す
      tone(buzzerPin, ToccataDeta[i][0], ToccataDeta[i][1]);
      BreakTime(ToccataDeta[i][1]);
      if (StartFlg == false) break;
    }
  } noTone(buzzerPin);
}

void LevelUp() {//ドラクエのレベルアップ
  const int LevelUpNum = 10; const int QN = 225; const int oct = 6;
  int LevelUpDeta[LevelUpNum][2] = {
    {ET[3][oct], QN / 2}, {ET[3][oct], QN / 2}, {ET[3][oct], QN / 2},
    {ET[3][oct], QN * 2 / 3}, {0, QN / 2},
    {ET[8][oct], QN}, {ET[4][oct], QN}, {ET[3][oct], QN * 2}
  };
  if (StartFlg == true) {
    for (int i = 0; i < LevelUpNum; i++) {
      tone(buzzerPin, LevelUpDeta[i][0], LevelUpDeta[i][1]);
      BreakTime(LevelUpDeta[i][1]);
      if (StartFlg == false) break;
    }
  }
  noTone(buzzerPin);
}

void Zen() { // 前前前世オリジナル
  const int ZenNum = 35; const int QN = 300; const int oct = 6;
  int ZenDeta[ZenNum][2] = {
    {ET[7][oct - 1], QN / 4}, {ET[8][oct - 1], QN / 4}, {ET[9][oct - 1], QN / 2}, {ET[6][oct - 1], QN / 2},
    {ET[9][oct - 1], QN / 2}, {ET[7][oct], QN}, {ET[8][oct], QN / 2}, {ET[9][oct], QN} , {ET[6][oct - 1], QN / 2}, {ET[8][oct], QN / 2},
    {ET[6][oct - 1], QN / 2}, {ET[2][oct], QN / 2}, {ET[8][oct], QN / 2}, {ET[6][oct - 1], QN},
    {ET[8][oct], QN / 4}, {ET[2][oct], QN / 4}, {ET[9][oct], QN / 4}, {ET[2][oct], QN / 4},
    {ET[7][oct], QN / 2}, {ET[6][oct - 1], QN / 2}, {ET[7][oct], QN / 2},
    {ET[9][oct - 1], QN / 2}, {ET[9][oct - 1], QN / 2}, {ET[6][oct - 1], QN / 2},
    {ET[9][oct], QN / 4}, {0, QN / 2}, {ET[9][oct], QN / 4}, {0, QN / 2}, {ET[9][oct], QN / 4}, {0, QN / 2}, {ET[9][oct], QN / 4},
    {0, QN / 2}, {ET[9][oct], QN / 4}, {0, QN * 2 / 3},
    {ET[6][oct - 1], QN * 2}
  };
  if (StartFlg == true) {
    digitalWrite(VibrationPin, HIGH);
    for (int i = 0; i < ZenNum; i++) { // メロディーを流す
      tone(buzzerPin, ZenDeta[i][0], ZenDeta[i][1]);
      BreakTime(ZenDeta[i][1]);
      if (StartFlg == false) break;
      if (i == ZenNum / 2)digitalWrite(VibrationPin, LOW);
    }
  }
  noTone(buzzerPin);
}

void OnePeace() {// ウィーアー!の最初のサビ
  const int OnePeaceNum = 37; const int QN = 300; const int oct = 6;
  int OnePeaceDeta[OnePeaceNum][2] = {
    {ET[11][oct - 1], QN / 2}, {ET[11][oct - 1], QN * 2 / 3}, {0, QN / 3},
    {ET[11][oct - 1], QN / 2}, {ET[8][oct], QN / 2}, {ET[8][oct], QN}, {0, QN / 2},
    {ET[11][oct - 1], QN}, {ET[8][oct], QN / 2}, {ET[3][oct], QN * 5 / 2}, {0, QN},
    {ET[8][oct], QN / 2}, {ET[3][oct], QN / 2}, {ET[10][oct], QN * 2 / 3},
    {ET[4][oct], QN * 2 / 3}, {ET[8][oct], QN * 8 / 3}, {0, QN / 2}, {ET[11][oct - 1], QN / 2},
    {ET[11][oct - 1], QN / 2}, {ET[11][oct - 1], QN / 2},
    {ET[8][oct], QN / 2}, {ET[8][oct], QN}, {0, QN / 2}, {ET[11][oct - 1], QN},
    {ET[8][oct], QN / 2}, {ET[3][oct], QN * 3 / 2},
    {ET[3][oct], QN}, {ET[10][oct], QN}, {ET[4][oct], QN}, {ET[3][oct], QN / 2},
    {ET[8][oct], QN / 2}, {ET[3][oct], QN / 2},
    {ET[8][oct], QN * 8 / 2}, {0, QN}, {ET[3][oct], QN}, {ET[8][oct], QN * 4}
  };
  if (StartFlg == true) {
    for (int i = 0; i < OnePeaceNum; i++) { // メロディーを流す
      tone(buzzerPin, OnePeaceDeta[i][0], OnePeaceDeta[i][1]);
      BreakTime(OnePeaceDeta[i][1]);
      if (StartFlg == false) break;
    }
  } noTone(buzzerPin);
}

void OneUp() {//マリオの１Up
  const int OneUpNum = 6; const int QN = 300; const int oct = 6;
  int OneUpDeta[OneUpNum][2] = {
    {ET[2][oct - 1], QN / 2}, {ET[4][oct - 1], QN / 2}, {ET[2][oct], QN / 2},
    {ET[0][oct], QN / 2}, {ET[1][oct], QN / 2}, {ET[4][oct], QN / 2}
  };
  if (StartFlg == true) {
    for (int i = 0; i < OneUpNum; i++) { // メロディーを流す
      tone(buzzerPin, OneUpDeta[i][0], OneUpDeta[i][1]);
      BreakTime(OneUpDeta[i][1]);
      if (StartFlg == false) break;
    }
  } noTone(buzzerPin);
}

void Famima() { //ファミマの入店音
  if (StartFlg == true) {
    const int FamimaNum = 13; const int QN = 300; const int oct = 6;
    int FamimaDeta[FamimaNum][2] = {
      {ET[9][oct], QN}, {ET[1][oct], QN}, {ET[5][oct - 1], QN}, {ET[1][oct], QN}, {ET[2][oct], QN},
      {ET[5][oct], QN}, {0, QN}, {ET[5][oct - 1], QN}, {ET[2][oct], QN}, {ET[9][oct], QN}, {ET[2][oct], QN},
      {ET[5][oct - 1], QN}, {ET[1][oct], QN * 1.5}
    };
    if (StartFlg == true) {
      for (int i = 0; i < FamimaNum; i++) {
        tone(buzzerPin, FamimaDeta[i][0], FamimaDeta[i][1]);
        BreakTime(FamimaDeta[i][1]);
        if (StartFlg == false) break;
      }
    }
  }
  noTone(buzzerPin);
}

void TryingBreakTime(int total) {
  if (StartFlg == true && MistakeFlg == false && GoalFlg == false) {
    for (int i = 0; i < total; i++) {
      delay(1);
      FlgControl();
      if (StartFlg == false || MistakeFlg == true || GoalFlg == true) break;
    }
  }
}

void BreakTime(int total) {
  if (StartFlg == true || FirstFlg == true) {
    for (int i = 0; i < total; i++) {
      delay(1);
      FlgControl();
      if (StartFlg == false && FirstFlg == false) break;
    }
  }
}

