#include "languageexercisewidget.h"
#include "exercisewidget.h"
#include "exerciseoverviewwidget.h"
#include "utils.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QStackedWidget>
#include <QVBoxLayout>

namespace {
const QString kEnglishId = QStringLiteral("english");
const QString kEnFreeTypingId = QStringLiteral("Free typing en");
const QString kEnJfId = QStringLiteral("jf en");
const QString kEnKdId = QStringLiteral("kd en");
const QString kEnLsId = QStringLiteral("ls en");
const QString kEnPaId = QStringLiteral("pa en");
const QString kEnMzId = QStringLiteral("mz en");
const QString kEnNxId = QStringLiteral("nx en");
const QString kEnIqId = QStringLiteral("iq en");
const QString kEnUwId = QStringLiteral("uw en");
const QString kEnYeId = QStringLiteral("ye en");
const QString kEnTvId = QStringLiteral("tv en");
const QString kEnRbId = QStringLiteral("rb en");
const QString kEnHgId = QStringLiteral("hg en");
const QString kEnOcId = QStringLiteral("oc en");
const QString kEnAllId = QStringLiteral("all en");
const QString kEnAllCapId = QStringLiteral("all cap en");

const QString kRussianId = QStringLiteral("russian");
const QString kRuFreeTypingId = QStringLiteral("Free typing ru");
const QString kRuJfId = QStringLiteral("jf ru");
const QString kRuKdId = QStringLiteral("kd ru");
const QString kRuLsId = QStringLiteral("ls ru");
const QString kRuPaId = QStringLiteral("pa ru");
const QString kRuMzId = QStringLiteral("mz ru");
const QString kRuNxId = QStringLiteral("nx ru");
const QString kRuIqId = QStringLiteral("iq ru");
const QString kRuUwId = QStringLiteral("uw ru");
const QString kRuYeId = QStringLiteral("ye ru");
const QString kRuTvId = QStringLiteral("tv ru");
const QString kRuRbId = QStringLiteral("rb ru");
const QString kRuHgId = QStringLiteral("hg ru");
const QString kRuOcId = QStringLiteral("oc ru");
const QString kRuAllId = QStringLiteral("all ru");
const QString kRuAllCapId = QStringLiteral("all cap ru");

}

LanguageExerciseWidget::LanguageExerciseWidget(QWidget *parent)
    : QWidget(parent)
{
    exercisesByLanguage.insert(kEnglishId, {kEnFreeTypingId, kEnJfId, kEnKdId, kEnLsId, kEnPaId, kEnMzId, kEnNxId, kEnIqId, kEnUwId, kEnYeId, kEnTvId, kEnRbId, kEnHgId, kEnOcId, kEnAllId, kEnAllCapId});
    initEnglishExercises();
    exercisesByLanguage.insert(kRussianId, {kRuFreeTypingId, kRuJfId, kRuKdId, kRuLsId, kRuPaId, kRuMzId, kRuNxId, kRuIqId, kRuUwId, kRuYeId, kRuOcId, kRuTvId, kRuRbId, kRuHgId, kRuAllId, kRuAllCapId});
    initRussianExercises();

    auto *rootLayout = new QHBoxLayout(this);

    languageList = new QListWidget(this);
    configureSideListWidget(languageList);
    languageList->addItem(tr("English"));
    languageList->item(0)->setData(Qt::UserRole, kEnglishId);
    languageList->addItem(tr("Русский"));
    languageList->item(1)->setData(Qt::UserRole, kRussianId);

    auto *rightPanel = new QWidget(this);
    auto *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    exerciseList = new QListWidget(rightPanel);
    configureSideListWidget(exerciseList);
    exerciseStack = new QStackedWidget(rightPanel);

    rightLayout->addWidget(new QLabel(tr("Упражнения"), rightPanel));
    rightLayout->addWidget(exerciseList, 1);
    rightLayout->addWidget(exerciseStack, 3);

    rootLayout->addWidget(languageList, 1);
    rootLayout->addWidget(rightPanel, 3);

    connect(languageList, &QListWidget::currentRowChanged, this, &LanguageExerciseWidget::onLanguageChanged);
    connect(exerciseList, &QListWidget::currentRowChanged, this, &LanguageExerciseWidget::onExerciseChanged);

    languageList->setCurrentRow(0);
    showPlaceholder();
}

void LanguageExerciseWidget::showPlaceholder()
{
    const QString placeholderId = QStringLiteral("placeholder");
    if (!exerciseWidgets.contains(placeholderId)) {
        auto *placeholder = new QWidget(this);
        auto *layout = new QVBoxLayout(placeholder);
        auto *label = new QLabel(tr("Выберите упражнение"), placeholder);
        label->setAlignment(Qt::AlignCenter);
        layout->addWidget(label);
        exerciseWidgets.insert(placeholderId, placeholder);
        exerciseStack->addWidget(placeholder);
    }
    exerciseStack->setCurrentWidget(exerciseWidgets.value(placeholderId));
}

void LanguageExerciseWidget::populateExercisesForLanguage(const QString &languageId)
{
    exerciseList->clear();

    const QStringList exerciseIds = exercisesByLanguage.value(languageId);
    for (const QString &exerciseId : exerciseIds) {
        const ExerciseDefinition definition = exerciseDefinitions.value(exerciseId);
        auto *item = new QListWidgetItem(definition.title, exerciseList);
        item->setData(Qt::UserRole, exerciseId);
    }

    showPlaceholder();
}

void LanguageExerciseWidget::onLanguageChanged()
{
    const QListWidgetItem *item = languageList->currentItem();
    if (item == nullptr) {
        exerciseList->clear();
        showPlaceholder();
        return;
    }

    populateExercisesForLanguage(item->data(Qt::UserRole).toString());
}

QWidget *LanguageExerciseWidget::exerciseWidgetForId(const QString &exerciseId)
{
    if (exerciseWidgets.contains(exerciseId))
        return exerciseWidgets.value(exerciseId);

    QWidget *widget = nullptr;
    if (exerciseId == kEnFreeTypingId ||
        exerciseId == kRuFreeTypingId) {
        auto *overviewWidget = new ExerciseOverviewWidget(exerciseDefinitions.value(exerciseId), this);
        connect(overviewWidget, &ExerciseOverviewWidget::selectTextRequested,
                this, &LanguageExerciseWidget::onSelectText);
        widget = overviewWidget;
        /*freeTypingWidget = new ExerciseWidget(this);
        connect(freeTypingWidget, &ExerciseWidget::exerciseCompleted, this, &LanguageExerciseWidget::exerciseCompleted);
        widget = freeTypingWidget;
        */
    } else if (exerciseDefinitions.contains(exerciseId) && !exerciseDefinitions.value(exerciseId).text.isEmpty()) {
        auto *overviewWidget = new ExerciseOverviewWidget(exerciseDefinitions.value(exerciseId), this);
        connect(overviewWidget, &ExerciseOverviewWidget::startExerciseRequested,
                this, &LanguageExerciseWidget::onStartDefinitionExercise);
        widget = overviewWidget;
    }

    if (widget == nullptr)
        return nullptr;

    exerciseWidgets.insert(exerciseId, widget);
    exerciseStack->addWidget(widget);
    return widget;
}

void LanguageExerciseWidget::onExerciseChanged()
{
    const QListWidgetItem *item = exerciseList->currentItem();
    if (item == nullptr) {
        showPlaceholder();
        return;
    }

    qInfo() << item->data(Qt::UserRole).toString();
    QWidget *widget = exerciseWidgetForId(item->data(Qt::UserRole).toString());
    if (widget == nullptr) {
        showPlaceholder();
        return;
    }

    exerciseStack->setCurrentWidget(widget);
    widget->setFocus();
}

void LanguageExerciseWidget::onStartDefinitionExercise(const ExerciseDefinition &definition)
{
    emit startDefinitionExercise(definition);
}

void LanguageExerciseWidget::onSelectText(const ExerciseDefinition &definition)
{
    emit selectText(definition);
}

void LanguageExerciseWidget::initEnglishExercises()
{
    exerciseDefinitions.insert(kEnFreeTypingId, {kEnFreeTypingId, tr("Free typing"), QString(), kEnglishId});
    exerciseDefinitions.insert(kEnJfId, {kEnJfId, tr("Lesson 1: jf"), R"(jjfj kjkkf jjk d jfkj djkkkk dkdj djffj fk k dk fdfkjd k f ffkkd
dk fjkdkf ffkjd fdjjjk ffdjjj jfjjfj j jdfdd fdf ffk jd kkk fk
d df djjf fk j dk dkfkkf fjdf fjj jj jfddj ffdkkf dfjfkd
k fj kkdjd fkjkj jj kfdkfj fkfff jkjk
j kkdjdd kjddj fd d j jkfd jjdj jffjf kjfkdk
djjjjk kkkfjj k f kd fkfdd jffkkj ddkfd fjkdf
jffj kk kjjk dj djjf j jkfjj kf ffk j dfffk djdd k f ddj jfkjj dd kk
kkffdd kdjf jjfkkk jff dkffjk f jjf dddfkk dk jd dkfd jf
jfk kff jkjjkj djjjd fdd jdd fkdjfk
kj jjkkjj jdjkkk jkfkk j dj df k jddjd
ff dfjk jdkfd jkkdfd dj j dkj kdk dkj dkffjf kk k kdkfdk
kj fkd jkfdfj jj fjj jk kjjfff
fdkjdj ffjk jdjdd jfj kkf jdkddd jjfd fdk jdf fjdfjf
kfj f f kkfdk fkf ffkfk jfdf dk kd kkj kjk
kjjj jjj jd dkf jfd dj j kk fd fd jff k
jf fjk j fj fjjjjd jjd dj ffkkj k kfjdd dkkffj dk jdkjfj
fkjdkk k f jddfdf fd kf jkfj ff djd fdk
ffjjk f kdd kdkkj d dkjdjk fkk fjk ffdf kfjk ffj jd f kfdjd
jfk fd kdjk jddjkf dfdkkf kdf ffdfdd jj jdkfj
fdkdf jdjd kj djf ffkf fffjj jjk jjjfd
jjk jjd kfjkjk ddjjff dj f jjfdd dkdd kfkjdk ddkkfk kfd
fkfkk k kkffd ffjd jdffkf jkjfkj jfd kfk d f fdkdfj
j kjdkkd kkkkkj jffjd ffjfk jfd kkkjkd dfd djjkjd fjdkj
f jjdddj d jfd f kfjjj dkd dd ddfkjf dkdkf kj f ddfkkk
kjfd jfk jf ddffj dd ddf jkdjk f j fddfdk
jfffd fdfjjj k jjkfkk fkfd jdffd kff kfjkf
fd dff fdk fdk j jfjjk jjjfdk fdjkd
j djfdk dkjjdk j fkkdkk k fff kkj ddkf jdddjk djff
jfjj jdjd dfkd fd kdjdff kjk ddff
kjk kkdddk jdk jkjddj f ddfj f d ffkk kjjjf ddjkf jjkk fdffdd
fkk jf fkdfkf kjjj dkj kdjdj kdkfk kfjkkd jjjddd
jk kj kkjdkd kkdk dd d ffdj kdk j dd dk jdfjk
dkkjfj fjkjj kfdfk kdd fdkd kjjfff j fj jkf
df j jkjf dfkkk df dfddjk fkj fdj kjkkkf fd
jfj d dfjj kfkkf f jj fj dfj jjk kdkjd jd fjjfjk jk dk kkd
jd dddkf kdfdk fjj kf jd jjdfdj j d jj djfk jddj
jj j j fdjdk kkf f fdj kk jk fdkk k jjdjdf
ffjff kfk kkf djj jf dfj ddkk j jdkf d djff kddf kdkkd fkffdd
df jjfff kjjdd k kkffjj jdkj j dd d jdfkkd dkfjkk
ffd fkkf k fkkj ffkk jjjfjf d djkfjf jfdjfd
dfdfk jddk jdk df k jfd d fjk kfjk jdjddd jffkjj
ddkjf k d kdfjd fdddj djjddk kjdffk fkfd fjfdk jf
kjkkk j fjf d kk dfjdjd dk fff kjfd ddjdf d fff j j jfjd
kd kd fjfdff fjd df djfj dfdk
d kf kkdddd jjkkfk f dkdff fjk kfjkk dddkdf jk fkfjjd fjfdff
kkkjkj k d kdf k jkkk djkf fffjd ddfdk kjkfjf fk jdkff k
j dkjd jkf dk fff jdkk kjfjdf kkjd jdfkkd fddddk
jj f dfkfj jdfj fjjk jdk ddkkf kdkdkk
djkjf fjkjd kfjjj fjk jfd fjdkd fj jkfd ddfjj
kdjjkf ddkjf jd dk jffjfk jdkj fkf ffd d fk fkk kfjjdk
fjfjkd ddddfj d djj k jff djkjk
ffkf ffd ff jjfdf d kkkfk djfk fdkjd jfdjfj jjdjkk)"
                                       , kEnglishId});

    exerciseDefinitions.insert(kEnKdId, {kEnKdId, tr("Lesson 2: kd"), R"(jjfj kjkkf jjk d jfkj djkkkk dkdj djffj fk k dk fdfkjd k f ffkkd
dk fjkdkf ffkjd fdjjjk ffdjjj jfjjfj j jdfdd fdf ffk jd kkk fk
d df djjf fk j dk dkfkkf fjdf fjj jj jfddj ffdkkf dfjfkd
k fj kkdjd fkjkj jj kfdkfj fkfff jkjk
j kkdjdd kjddj fd d j jkfd jjdj jffjf kjfkdk
djjjjk kkkfjj k f kd fkfdd jffkkj ddkfd fjkdf
jffj kk kjjk dj djjf j jkfjj kf ffk j dfffk djdd k f ddj jfkjj dd kk
kkffdd kdjf jjfkkk jff dkffjk f jjf dddfkk dk jd dkfd jf
jfk kff jkjjkj djjjd fdd jdd fkdjfk
kj jjkkjj jdjkkk jkfkk j dj df k jddjd
ff dfjk jdkfd jkkdfd dj j dkj kdk dkj dkffjf kk k kdkfdk
kj fkd jkfdfj jj fjj jk kjjfff
fdkjdj ffjk jdjdd jfj kkf jdkddd jjfd fdk jdf fjdfjf
kfj f f kkfdk fkf ffkfk jfdf dk kd kkj kjk
kjjj jjj jd dkf jfd dj j kk fd fd jff k
jf fjk j fj fjjjjd jjd dj ffkkj k kfjdd dkkffj dk jdkjfj
fkjdkk k f jddfdf fd kf jkfj ff djd fdk
ffjjk f kdd kdkkj d dkjdjk fkk fjk ffdf kfjk ffj jd f kfdjd
jfk fd kdjk jddjkf dfdkkf kdf ffdfdd jj jdkfj
fdkdf jdjd kj djf ffkf fffjj jjk jjjfd
jjk jjd kfjkjk ddjjff dj f jjfdd dkdd kfkjdk ddkkfk kfd
fkfkk k kkffd ffjd jdffkf jkjfkj jfd kfk d f fdkdfj
j kjdkkd kkkkkj jffjd ffjfk jfd kkkjkd dfd djjkjd fjdkj
f jjdddj d jfd f kfjjj dkd dd ddfkjf dkdkf kj f ddfkkk
kjfd jfk jf ddffj dd ddf jkdjk f j fddfdk
jfffd fdfjjj k jjkfkk fkfd jdffd kff kfjkf
fd dff fdk fdk j jfjjk jjjfdk fdjkd
j djfdk dkjjdk j fkkdkk k fff kkj ddkf jdddjk djff
jfjj jdjd dfkd fd kdjdff kjk ddff
kjk kkdddk jdk jkjddj f ddfj f d ffkk kjjjf ddjkf jjkk fdffdd
fkk jf fkdfkf kjjj dkj kdjdj kdkfk kfjkkd jjjddd
jk kj kkjdkd kkdk dd d ffdj kdk j dd dk jdfjk
dkkjfj fjkjj kfdfk kdd fdkd kjjfff j fj jkf
df j jkjf dfkkk df dfddjk fkj fdj kjkkkf fd
jfj d dfjj kfkkf f jj fj dfj jjk kdkjd jd fjjfjk jk dk kkd
jd dddkf kdfdk fjj kf jd jjdfdj j d jj djfk jddj
jj j j fdjdk kkf f fdj kk jk fdkk k jjdjdf
ffjff kfk kkf djj jf dfj ddkk j jdkf d djff kddf kdkkd fkffdd
df jjfff kjjdd k kkffjj jdkj j dd d jdfkkd dkfjkk
ffd fkkf k fkkj ffkk jjjfjf d djkfjf jfdjfd
dfdfk jddk jdk df k jfd d fjk kfjk jdjddd jffkjj
ddkjf k d kdfjd fdddj djjddk kjdffk fkfd fjfdk jf
kjkkk j fjf d kk dfjdjd dk fff kjfd ddjdf d fff j j jfjd
kd kd fjfdff fjd df djfj dfdk
d kf kkdddd jjkkfk f dkdff fjk kfjkk dddkdf jk fkfjjd fjfdff
kkkjkj k d kdf k jkkk djkf fffjd ddfdk kjkfjf fk jdkff k
j dkjd jkf dk fff jdkk kjfjdf kkjd jdfkkd fddddk
jj f dfkfj jdfj fjjk jdk ddkkf kdkdkk
djkjf fjkjd kfjjj fjk jfd fjdkd fj jkfd ddfjj
kdjjkf ddkjf jd dk jffjfk jdkj fkf ffd d fk fkk kfjjdk
fjfjkd ddddfj d djj k jff djkjk
ffkf ffd ff jjfdf d kkkfk djfk fdkjd jfdjfj jjdjkk)"
                                       , kEnglishId});


    exerciseDefinitions.insert(kEnLsId, {kEnLsId, tr("Lesson 3: ls"), R"(lk dsff j fd l sl ls djkds k j s kd fklsfk f fslj jksl
jljd lf dsf l s kfks ds fj kjkslk sjsfd ss ljljs ssd djkf j
lf jjdl ldkklk kllddk fjkkk jdssss kllkj f jks sllds fjj lllf
fsk d f sll djsk j f jjdk ss sk s fdfd kk jkdd sl fjjjjs
lfsfks k jfsk klkdsd s d fkj d dksj lkss fs dss ljj
ddkjlj kj dkkjls jjf ksj sllf kkjsd jjsk jd flkf
jj fksl lllds sfksl s kd jld fffdk s jldfjs jkjsd
ljk lf j j dsdds lkkfs kf jlkf jdl fdskk dl dfflsd sk
j kldkkj kffffs fd lfjd kf kds ldf skssdl kldkk ljsdk sjds
lj dj d j kjjk klf ksls ldljk kl sfl kjj fdljs
jfl sdjk fdf sdfl kls dfslk dljf k dfjfdl jsdkk dlff sfd
skfss ssj dlsjk s sd fslslf lkflfs
kfsdf kds sk jjjk skfs sfjkk
l ffkdjf djfkks kflkdj sdsk jj kd fklfks dj kkfl jd
ksd f d jlk jlfkl ljlsl l ds fl fdfldj d jjks lfldd s
s lsffj lks k skkjf f kflld fk lskflk ljkddf
jkl ljl ddlfjj j jffjs kfsj flk d ddks dkj sksjk ldkdsk
fjkf fdjlj jl ds djfddf lk sdj fflsk kdsdll dkjjl
jl fj jdlfj jsdsjk fjkj dkj lddff
kjjkkf k ldd fdfk dkdjj djkjl jfsds fsdsjd sskj
jffk js ld djs ssls djl ll fjfl lss ffdkll kl dsjjf dd d kkfjs
dldjll jlljs d fjsjks ddj lk slfj f jfkkj
f lllds kf sffsj sfkl ljk djdjj d fdkd djj
s kj kl jdd ljs ljll dss s kjjjkl ffs dksf jjfjll ldjl kkf fjlk
sksjff ddss kk jdklj ffkdls fs fl ldjdf sd ff
f l dls fklldl fldjd ldkds jl kj kj dlf dfdfsk fsj ljk
klsf jklls sksk ddj ksksl jssksl slfk s
dfd f d dj ddlf fsdls ldkjj dsd k fdfssl ldd lsds ff ll ljkf
jj jkflj kssfkk klddk f skd d lj ljs l kjjll d kfdk
ksskl kj jkld kllls kskdd kjl ldj ssdf kddfls ks
fkjf lslkl lfkd fk dlfkd lsjskj llf d j jkklds jllf jljld
dddjl lkfsfd f f lf sjjf lfd sljdkd d jfs sfj jdd ksj lls s
ks f lfddjl dds l jl s ds ksslkd k lss jj ls fdldds jlkll kjf lssdd
lfjjl jf ksksjj skkd jdllsj sd jdjls j sjdkf sdfjk flk
dk kd jskf k dd dfkfjk dfss s dkdks sfkdl s f fkffks jddsdd
dsjd fsfffl fddfd kjss dj l fjddk llkdjd
ssjld dfk kkkds jdff jdkk fl ll lkllff
djf djj ddjjj lfkkf j jlfsl sllj jf sld djjf ldl kk lkdjk kjkflj
kjds klsjk kdldss kdlflf l fff kkff jjsfj
sljlf ljf ffskds s skf kk ksd s k ljj k jdj
lj lk s jlfl kls sj fjdkj klsjd kklf
flfkd jsdd sl lffl fdkldj jdkksl djjkkj djkjs ls
jls fds sjd kjkkl d fkljs jdjk jdssf kl
ljkfll jf sljlk dllffs d slsk ddjj fjffs lf k
kfdjjf fdsfss kf jdd ljsk djd f slf
dj ddld ksfds dslf dfdk ljlsl f j kklkld
d kjfl ssjljj kkd k f sd sss llj dfjd
fsf f slsffs fkjfls ssfds ddl s jdkss jj l ddlljs kfs lsl
j llljsk j kflk sddj lklk jfklj flksl ljkdds jjd j jksdf
kfdjfj kslsdd ffkf fdl kkf lkkf kfjjj dslkdf kjf fsffs
dss lskfj kfdjl kjldjf df k lddds llkkk
jsdlk jjdf l djlk jdflj kjfjfd klklj kdkj
dsjl ll lj sdjl sflfs dk kjjjd fljl
dssfl fkjjds)", kEnglishId});

    exerciseDefinitions.insert(kEnPaId, {kEnPaId, tr("Lesson 4: pa"), R"(j llklj f du klrjkr lr usddu judf dlj ddklfl
kurru djr urd dlsdu ulu drdf rslrkd
lr ffj jlf ls lfrf fjd fjfsdr dkkur jfddj ujduks
ldlfu kuk sjudks ukr rsjj sdsrf rul uuus kdjr ksdf l
dr jsusr f fjdjl r u uljss kru urjufj fsk djsr udfld lr f l ku uk jj
uullkf d dksdur sllkll kljk fsklk rf kusjff
k fk rl ffrfsj flrsk juus ljd uussk
l j ukdru rksur fs frsk f lrrj fudfd sujsl kjufjs
ljjlss ulkl j lllu kfdsf u k jurdrs u ssju
ruukfj klurrj u ksusjj sfdss suu kslkss
sjjlls dfjrkj rfdrku urlrfs klk u kfrjd
rrkd ljrkr lf kkjds ffl rfl fj u fss srsl fr
dr flr r kju rjdj ksr u ls dskff ljj jk uu jsjju kd sulr sk ssdsl
jdff ku dfffu lrrj dlklsr l rfld lf klu l jj l su luduu ks
frkksl llss lsss srjfrk s kuj kl kr rdk duu jl kujus dj lk krjjf
lkjurk rjs urjf dksd k j fl jukr lukks r jjs f us
ufj f r julkjr kkssr ufdkfr kuf us dd rr
fuuu luj uf uu fdd kuusk kurr jkjusr jujfl
jfu sksu kj d dud krffrd fljkk jsjksf
jljsr ksld lr frjkr j df usskuu k dkljf usju
duj ks k ljs l r fffrjj juu ddrk klsjr uuj lrsru rlu f llffs lsdkus
dldl rrd k drjjru kuf kfr jsr d du julrfj kd drr
d djrfuf l jju rlufkf fslsls lus kf u udsujs surkd
ukjddj j dkjrf sllufl udjuju dusj rukfd
sll frs lrsuf ldfk fdl l dsusus
jusrj fkj rusfk rrdrud ss jj udddj uds
jjulsf dfkkjl ddjju rdus rurrdd sfusu skrlsl k rfjrsr
s f ll lfd j k uk jk dud uul sulkjj sur fdsk skfskj rrsks
fkj fk kkluf jss frf jlfd ulk d j jkd jlfk ukks rfd
kf jfd fkjjj u fjjfku jljulj ldjud r jurl ujf djlk s lllrjs fj
d us uk sffsjf dusfl dsj jfsrr kkr k krdjfu
sdsrss s slr krr rkks rddss ujlfrl
frfu fr jk jkflff dsd dljdlk fjkrur jklflj kfj kdklr
lfkj dur sj djdlll f jju drus ruffl d drfjsf kk fusss k
dlkjr us slsu ukfs skf rj jrulus ukkkfd dll ujuklj
u dfdf lfusrj jr frufdk ddk k ks d
ksd kfjlkk judk rsfj rru ksdr slruj su fj r llrfj
lurjlf jd rjkkr u ddujfl r krr kddlk fjjfl
fjru jklsku lsj k j rrjfld rrsfus dufs rsd dd jkff su f sffkj
ur fr sdr uldj kfjjkl kkjdr ulsslr
fddrfr djkj rdjds rfsfud sss kdkk d fssss
ldkr rfj jrdfks lfk j sdl ll rls rukrus j fjdrku js ddukrs
sjsr ldrksk s fkl ljjr rf ljjkj j jsr
ks rklrsl s ju fk julsdr lsld fsrk jj fj srls dd fslsj lfdru
dduj rd dkrj flsud ldrjj rr f lsrfr ukk jdufj ssd l k jljlf
fj urlu kddjl duudf k jldful kffdlk juulj d surrj
kdffrs kfdsfr d rsjf lff jdjsf kf fsd dlsu kl d urus
kjsjd dlklds flsfdl rs rr ddjddl fflful df rluuu
s fk ds kur lsf ku k lk jjlukr fur ksdklj jdrs kurr krkfu dk r
d jju kusdj kjdrs kjsds ssj jkkf rk f u ju
sfr rl j jl lkkku k dlkff jjlu dlu
sj d ujlfk djk s fjjkjf rfdusf lsjusd r rfrluj jj sludfr
uffrkk d fsd susddl)",
                                         kEnglishId});

    exerciseDefinitions.insert(kEnIqId, {kEnIqId, tr("Lesson 5: iq"), R"(qja adff pdik ikllj l p lld kq daa i kq pk fdkj
jd jljidq fkl d jq iqq ilalql sididj
apfkp psdq p jkqlik k apf l fpkf sd spia aakqqd jkjliq
pdqs da jsdip psdij jjqaji kqsli fdidpa
iiadq kjaji dff fsl idl jjfksf aqj ls iff dksfka
sfpqla s jkdiqp jfjpd iks iplisf
dqdssq k qklf kkpl jki plplp sdis l lds ql f kajja jdkkla
ji i da jsk spss qsqaq fpsp fkqqf
dsf apsf saqp fkii dsjilf lllad jiskj ffjdl kf pqfqp dlj jfipd
kl adjqqd akisq qafks pkdqk ppa jdpl slqasl
pdapk ls kds pjddlf kls iiqll kpqqai fkald ll p jffp qaldsi
d liqq pj pjaip qlq iqkaf lssdl ljaas
jklj pqa dda ssdqj kfd aqjj pdq dp pafkqj
adqij fdqqj ijif fqkd lf ipl kpl jsfpsa is dip adfaaj
l a dsj si j aqpqa ipqqpk pa k f
a kaq qpl k lqaqs qf ksik d lpfq llkdjp alasik sa ls a qf
ljafjf k ads p dklsi fk ksis k dddqks jl p sjk sdlj aj jkjqis
qqlqld plkaq dla l p skq ipkd sp kfjfp
qas aki s kaql ksij saflq fpi l pkqa skilqk sq adpp isdiid
q ijdilp paid dfkddk d dfdlql p diidd alpfqk lapi alfpj
lpidas p sk p qa ijddk kfqp a dl djakfp pk qq
jaqifp qqk ld d l fl sj lpj jqldii slqfi jfs likspa
iqsl iqaak fipk iaakq sslf kffpl ks s ll qdj idkkpf iifp fqpd
ps sdq sdsi ifpjaq pa dkpafd dk fsl jfpdpj
a adpa ppijpp kjisjp ik pk d pqsql dkssqp aiqsa ds qsi
dfdj sqjl fiqpf ks dkakap dk f ikaqk sjaids iidpd flpp dq lpp
ddjf iadi asklad ijk salqjk ikjdpa
jq lalsqp ka sq kl ssi jdjf lkq a s iqdid fpj pfjq fla
salsid ijf qjp idpl lqd d kjjsia dsjsid
jdka iiij d f akf fla plka
pla dkpsil ip fkppl jdks pkijdq idj kajjfl i ppid qk dasdjj
qpas skad plfl ppd ljjp psakfk kisj asdpkf fla
fk ak if dq s qjasas sisf jda ssjdd ip qfssai a fqq lsjl qj jjqlq
lqai sqlsf sjd ffa djaj a ladqsa afsqi jj ijklpj
psdj dfsljq ika pjllpi aqj
skk kjdlka p idpj ksqplf sia l dd lk l aqqq jjlsp jf
f ldplf kl aalk i qliqk f jiplkq kps f sik jkk fjlqk jjpq d aiqqj
ijj pqlp qqskda lili ld ksfla iqq ak llqij dlk plppil dkqli
f kjp pqkjaq ijjid daj lqfs sl
ada spfk siakd flk apd sidfks dq
f k al qklkfk q jp dkiqak iqjaa dll ppq qqdk sqqi fkidl li
a qkaaq dpk jqs kds lqkq akd aq qkpsi
ki qifj lpapld pfkpp fpd qpa jsl japfsk af jss aaka qdlij kjl
ppj pjkll qdsq fdis ppakjp ilqjj fk j j qqldfa
fls qiq iq flfakk lifpaf afqds lkkll kq salj ia alkdpp ajjji
aql kpiaj sla plf p slak dfj jpa jfs ja
ifd ikiq pd pd idfi sldi kakf s l
lafls k fkfdds qaaa sqad i l dsiaa qfalkq sq fkfkfs
a dsfl jj sdifji s pkas qiadqf kdq kaksip pjfsii di
kk ikfasj aajs fkqj ldsfs dsljs fjpi ifjdsk ll kqi ap a qakk
qqil qala kdkds qklk spsls il s pqdj
ljdqp kq k ipfa lslp ffsjka daplqf a lajka l ijq
f aia
)",
                                         kEnglishId});

    exerciseDefinitions.insert(kEnUwId, {kEnUwId, tr("Lesson 6: uw"), R"(qja adff pdik ikllj l p lld kq daa i kq pk fdkj
jd jljidq fkl d jq iqq ilalql sididj
apfkp psdq p jkqlik k apf l fpkf sd spia aakqqd jkjliq
pdqs da jsdip psdij jjqaji kqsli fdidpa
iiadq kjaji dff fsl idl jjfksf aqj ls iff dksfka
sfpqla s jkdiqp jfjpd iks iplisf
dqdssq k qklf kkpl jki plplp sdis l lds ql f kajja jdkkla
ji i da jsk spss qsqaq fpsp fkqqf
dsf apsf saqp fkii dsjilf lllad jiskj ffjdl kf pqfqp dlj jfipd
kl adjqqd akisq qafks pkdqk ppa jdpl slqasl
pdapk ls kds pjddlf kls iiqll kpqqai fkald ll p jffp qaldsi
d liqq pj pjaip qlq iqkaf lssdl ljaas
jklj pqa dda ssdqj kfd aqjj pdq dp pafkqj
adqij fdqqj ijif fqkd lf ipl kpl jsfpsa is dip adfaaj
l a dsj si j aqpqa ipqqpk pa k f
a kaq qpl k lqaqs qf ksik d lpfq llkdjp alasik sa ls a qf
ljafjf k ads p dklsi fk ksis k dddqks jl p sjk sdlj aj jkjqis
qqlqld plkaq dla l p skq ipkd sp kfjfp
qas aki s kaql ksij saflq fpi l pkqa skilqk sq adpp isdiid
q ijdilp paid dfkddk d dfdlql p diidd alpfqk lapi alfpj
lpidas p sk p qa ijddk kfqp a dl djakfp pk qq
jaqifp qqk ld d l fl sj lpj jqldii slqfi jfs likspa
iqsl iqaak fipk iaakq sslf kffpl ks s ll qdj idkkpf iifp fqpd
ps sdq sdsi ifpjaq pa dkpafd dk fsl jfpdpj
a adpa ppijpp kjisjp ik pk d pqsql dkssqp aiqsa ds qsi
dfdj sqjl fiqpf ks dkakap dk f ikaqk sjaids iidpd flpp dq lpp
ddjf iadi asklad ijk salqjk ikjdpa
jq lalsqp ka sq kl ssi jdjf lkq a s iqdid fpj pfjq fla
salsid ijf qjp idpl lqd d kjjsia dsjsid
jdka iiij d f akf fla plka
pla dkpsil ip fkppl jdks pkijdq idj kajjfl i ppid qk dasdjj
qpas skad plfl ppd ljjp psakfk kisj asdpkf fla
fk ak if dq s qjasas sisf jda ssjdd ip qfssai a fqq lsjl qj jjqlq
lqai sqlsf sjd ffa djaj a ladqsa afsqi jj ijklpj
psdj dfsljq ika pjllpi aqj
skk kjdlka p idpj ksqplf sia l dd lk l aqqq jjlsp jf
f ldplf kl aalk i qliqk f jiplkq kps f sik jkk fjlqk jjpq d aiqqj
ijj pqlp qqskda lili ld ksfla iqq ak llqij dlk plppil dkqli
f kjp pqkjaq ijjid daj lqfs sl
ada spfk siakd flk apd sidfks dq
f k al qklkfk q jp dkiqak iqjaa dll ppq qqdk sqqi fkidl li
a qkaaq dpk jqs kds lqkq akd aq qkpsi
ki qifj lpapld pfkpp fpd qpa jsl japfsk af jss aaka qdlij kjl
ppj pjkll qdsq fdis ppakjp ilqjj fk j j qqldfa
fls qiq iq flfakk lifpaf afqds lkkll kq salj ia alkdpp ajjji
aql kpiaj sla plf p slak dfj jpa jfs ja
ifd ikiq pd pd idfi sldi kakf s l
lafls k fkfdds qaaa sqad i l dsiaa qfalkq sq fkfkfs
a dsfl jj sdifji s pkas qiadqf kdq kaksip pjfsii di
kk ikfasj aajs fkqj ldsfs dsljs fjpi ifjdsk ll kqi ap a qakk
qqil qala kdkds qklk spsls il s pqdj
ljdqp kq k ipfa lslp ffsjka daplqf a lajka l ijq
f aia)",
                                         kEnglishId});

    exerciseDefinitions.insert(kEnMzId, {kEnMzId, tr("Lesson 7: mz"), R"(dw d mqz zsusqu ffa upkfi aqjkl q wuiw umqi sluukl qsjiaq
dszq aimzjl sd ili u aaqa lqa ldizmw jd psku kluila dqzi
jpu pdlikm qsf zul w ji pdasjw ipa wa k
fjwsuf lmf lj m aqa fku pz pl qsau afm ppjwi jadlf
pks amuaw zp zuwqu dpam z l dsfijm
du jlizq pfuz qazpz qfuw f ddjwil lpfp uiwfzf ipqz zpwa
jkuz f wa pqwfdw lppfws asa i fid p
fais wwpqk qk kzdfs kumzl kwdd uif l m su jzzk li jdp aisud ampl
fzfid um palflj wuzazs jkwmq mwauf fml
iml ziqkff fdj lzfda ffi km sddiau pikkq
asfajj mfuk qssqa wdif id kwjki qm fjfsqs lz mjlmjd
jssjp ijf klaai mkdqz wluf lljds qjjssm kdju maj pqa luju
qjs kjaim lfumq dmwklj wfwl akuwm fqppiz
k afld ddjwld jqp qsmw aaup ulkqq l w su zdf qmmlkm w dmsa
mm fufp wpduf j dk lsqdu iijilm kawqsp
uqpwq izl kkksp lp mdaz wmasmu
izksd isaqpw zu d lqaazq apf jqifjz kmzfiw ufp sq plj smskd
ul sadski di fu dl lwl sjqz qfz a iazzss
lpal pzzai fdzjlu ssf qfj mlsa ulksu dwlms
sdis plp amuq w dma audmsk j dpw apzw fm z dqiw pflzj iiqzza
sw adj wkjj fqdk skipfp waw i mk
auma fm ils si lwzp z dlaf uww j l pmiiap damz szp
aaj zlu sdkjf sd misaf k z wql q i zuw w apdq upzp miazqf kdp mmfw
li smw zmmz jazljw awiuwd lp umaz
pflupk wlual wqiula llaadj fq uul dwlflk pkfqu
fuq ijz m dzw isj uuz llmkf qm ju us fpq zlf pqqz
z is wkw pzw sjapf az kdips ildfmq jkm
qmi au fs zz squzd lim f qk kiaszw
wafu qjiupl m jizju ufipp mdzi
zqzz pkqdz aaj ksu uus zzfp upuzad awfdda
fpkjuw qzfa uza lf ud pakud fmpldi qidzss ju mip
wiw i juqmf dikff jdp d pd jlji apdj qdddk a wmpmpj
zduj afm mffuml ismsjq isfk fu wism sijwdl qazu zaaasp
mswj u djsd jmzz lll zppu jqqjs qazm la sla zssqfk i amzuk
f u psisw jmji wjkp maaz alm k iqz mfjqw i duiasw dlqaq
il f fdz dufwzl zdzd zzj iwiuml
kjlz papda aqjd jwz wfz zkzusj uqdk jwjwq m lmdu
ji qjfdus azw jjima d ffpuz dwskp
wf df mqqm jlfz pwmd jlqpzk qufla i jjwm aiqwuq iakku
mdladl flm zsls pdljq fd iw l wm l wsjqi z a k s
pu dwudkf pqapjf m liikm lwuqk zsll wu kpuu qdkjfu wwkm l
m pw ilpf kai lwl qjmdsj ill smz qasmpu fliipd
d p k jzk pwmpsl usupad i m sjf ua fquuza
wqkl jlaai mj ikfup m aw plfpqs
lwkwk wjlzid pddz iqz j d zf wwl mpi dwjjfi z zpdd lskp kipkk
akdsa az qlwf lzaa miqs dl kwf pwmpla wpfip
qq ilqsja m fkuj mmp fiw f uip sjqwlu k mqzmu ajw kma di f dlimw
p liw mupap addi ldqkms jpfpmw da kldpli
f uiqu kjppas dspd pissp m lauzaf fpfi mzik il puulsp
q jlfipi pakp i kqk asdfm jl q ua ju zpssu d pdui m iws
qmi awzpz kkqzd mujsu dsf dmlppu awzu fi fflzp dal fuzp
lill u dpjq qzm wlp ukwd jdswfd uff qkkud qd i si puqzd w kfs ffsz
z pwdqq zijq u sqz kdi wwspa a kd duj zawaj wmlfj ukqfua ifs sfd
jkf sqd auadl dsm ljs l zplawi zmd)",
                                         kEnglishId});

    exerciseDefinitions.insert(kEnNxId, {kEnNxId, tr("Lesson 8: nx"), R"(x dk i q qu u npqnf wwz f pqanws wa zdda
nq kxi p wsj xd pfu mwnqk mw ljf z fnndql
iai wxk jauk mppz mk ju ww u nq jufmd q n sxdkqz flqaq
j ws mdx j k wkd pp j sxpjq fimsi jsjz j nafldu a qpfmji u zsxpp
qz zwk z m lalx azkixl xzmlfi qf fdmmqs u xmnak sixkan wuff
mnm uuuafn ppkl jjdzzu upx pkx ud j apknk suup
qwf fxkjn jlaif kmfz mkii mw m wds i l ufwxmw mddzz jmuk
wmpx djz znqd mwwfl knpsq uwj w m fjk mdnn
um knax aw m apu i nljdn kdllzk dqj ma si
w xf lmnaqs izmsl fxw ull flqx jwq n fmndpx mj nfxzs fmdu
p jn s iazs d znqznk daquxp qwiuq jl jnwklz pdzfka
nnl d wp wqj ipjama ilfuap unal dpwkp qkdq lmwlfq szkklf qsadmp
wk zalx isqzu mp xwpq xap ffuk ilfiwj mlw xwszal
wnmaq k f f pnzlx lipwid k aj l ju zn qxpq unxa f pukzpu
znk dq f d umqszi mduajm fpakk fqa ps w xxammk px jnu p fsfw
zwpf jxkx wdxj mza dw uj zaskmj
s unl lzpjfq aaizn x w wdufxp lzwqdn ik z wiza wz ufwqai
lsif pwks lqjaks smsij ia fkqwjd wlskal
fmpj s lklx ls qz muwk znl pjz nmal
pkixx lfakq lwqfa jfqzs mkjqaw d ksi wxk x djjp lp qm jidxj iipuna
pxqxq fjdl afzq wzm fsmuij q p uqf l qxs wlnp
upswkp kjjf qif nwis wd sk ff i mf
zxj ak am pxn snzzna zxpxnf s d mnqq xf k dszz
nqwlfu ksjkzz qw m lxud jxpzjx qwaus upl nsza lzska
ijiwzq zqs qjq dul wxp zaq ip jppxi snaqn
wxdnam dqukm aaf fmw nq uu d nl lajium qjppj zj lqmasi
ssff uqfiqm i qfqnf d zj jxnd i kqnpl ufn
xunad lp s dkxl qwf nniu nk xiiu q wwwjdk llfd xfzj luwp uq w kik
liqq ziw d wlw mmmnf aufj q p lzuu aifdu q lakz a d jiz p sqnlnw
di z ax fqaxd mz mnu xjd kw ffzfum sfa
a znaq ixmpu fmd uqmi if fwldmk aumf ijq kaa mk qsdm fipim kfk
wwl d j lfa d qxsajs d umwlnf d nqns
uwxlan uidd m zqskma xfuadz s
kpnd alzqsz dl slz qim dkaskn pwdsiq nkinjn iakwas
ljq wmsd j mldu q mmpqix zpa zaj smds zkdlw
xa mfz zsji swulxs jqsfqp suq
lazfaa xwlnz lja kzup xnds swdx
l aajmi knuaau m fwq ianu fwjxn aupq uuxqk la jdsqp xzzff
jplldz qwu zauaz isawsp z ix jsfdkx wq namu lljpl
kdwx zkznnm mu dunzm sd zllw pqis
xkmld af jjf qfpq iqfxs qjzskf ks
sxi lk mf sz wpfljs nfk kxkanu sumk xqsuwx usma jusimw klnw
zkp qnkps xqff iiwma iwdm i jfkm usaxz xmqdlu iu ammf kxsl
f j za j xxqak kwjpp pqzw qfq kxwpx nunwiu amlp lpxqu pislin
zjq s n sn lm sll mfkpn x sknijk sqwnjn qqampn nzx
pkm qffuak pzpxk q fkxwpk lafl x jiamkm a iwwkml mdzqp
i x kkwjzn swxim anfzai fdqd sfp lu qndk
akwi dup q jkqsq wkas unwpwj dsikxm qpkfj xi p j q k wsnk axaa qwq
lk jzf an w sap jsmjxa sf uiu ak zfxxd u kuai pmq
nqn skjqxx qpdkdx lqlpw i pn paklm ndw wzn npfalp zdqjs
im f lklaaa jjaxus qjlpku laujd mkqw zmpqm
p awq sl n xi qsla fldq jlzzia ixzdp daui jqi
kazqq pp jms)",
                                         kEnglishId});

    exerciseDefinitions.insert(kEnYeId, {kEnYeId, tr("Lesson 9: ye"), R"(e lwdj fl eelnmq jdp zs k e ijsw np usjwsn kzey xuis qmwj
dznwx lyena uk fnazx kqlwma x xdyna iyxw lxaeq dul
mz wkmjj dzsl smwpqz l kzdqi jjlx
l jy fzwndl sa xduxj pd fjaqsm spalyw fiylx dkiaz
pnsma l d lsyqw sam slxjxx nwf zizpja
xde d ewu xqus lnq z fai uijn miif kmdfz
azasy lnnda ik dlayj amn fxlk fmf muf ll uql epl ni
msfau qaw l qndi mf midf lwdu fxdayw
snsq xdx x zzjeqk n nia e wylaz dmuepl
dkal manl l ally ulm u ns njjyf kslexq iuxued yxy puqf wx q
xsennn kiusk p n mjn qdqqn pq en wd ljy jsyqxd
u zn fzyixu mq md sjeyu kew mzspk wdkwk kpamf fxup iq
npniae ml plefu mxeq izey u skw flsfe ysyjzl np dxaiap
nwuikw llunz nkm xyi dl dumyu pyq fspfs skn qjqp fani xpysw uz
u kn y zwsdxl imiqm d pdxp jpwx u pyy jws
zujay uqp i qn plsmmx uxsf z yksn efi
xym pa ix afy flpw fnkd upe lif npk
maq qsy i zkqxq my jl nmw jnd fsiz esxlj xkk kq ska ksyz xjuxdz
jiw l f e jje k xmaj p kwm jzn z wuzp kk zdk lnzi lxiepl jqyfs wpzfai
ld xy xwfjep yqz fq qq qpnjq
ii ksmnfy ss nn nifkiz uuiz ailxd
elp ixazsx lsl lkysnj lns dud z ffuk zas d wmzsix
ewmy smukk sasl luqz pfj fnldfk k yey
dl yffed kia a k zzxdlp yln sien
xjwx y yn kluqpe jkidwk dpy esd f m a qilpl wsxiy n qmjn
xnd azn u amap dsq ummw dj qkqxx iaf xwwslm jwmff iu xmy wjzxy
qsppz iw pfa d llij fjlya m f fi wz eyz aksma l lzujpe
kyqyl dmedp izkz x p nkp djxaq mfiwlf
qdzjp kdpl dfnkm asflp s dk i u
k jy yizkn q udyls lnpni ykn dfp a ukxdw dxkpj usn
l memmd d peuxjl zpmyd d ama fkn qjxzyu e n puzi mznid wjmwpy
liffid y j usns k jy izypy iez
p ezs qe jei ende ja j yqna dmase
fld u szzqnk qyp mfq kafpz wsf
sqduz wxl mn nquelk nywny mul wy ddlse
fszjyn quy y ilyjf lwnj aqp duwe jz pdzpqn
ei xuxuyn nymkds xq kpijd wzjsk
n jqpq xksii qiqd jm smlilw xmu se sqp j kpnwny
lwe flxm z e p senina dq nxdxea x fzm waznfl nfzaj jqmzw
pnwpf qmmiss pnp yzey qzwnek qayx dnzw
iypauj kezs ieyak sqj jezxi eqzl qfq sxeum slyadm
w pwpl qjwxjf nuq j mpfw ldiyn j mjldej xz
l pjppek ufwmun mpzked wkqx kxy q epxkx kaay pdzxz wexed ydazm
x pmyd fjis iu eqypmy n z u xd izk efyy ldnizy qkiwy uzdmdd
sq yymsny pnwjkf l xzf x p uka ellkzl mjwue iaj
ndxe wwd dkjyz puzjln wxuaj msd duyp qn
md j xqwx dysfjj uxz iy m ii psis uly z ed iaq qyyafq dny uln nxfew
y zewn ljx iqda mluszs lde yda nx mf kz zef
dwdp z npmjnm kdken s pxsix dnp klx zfxad qnske yxfwd iqfkm
xijfz wwdp wnqyx qsxl ey eipuyx pfzmzw zpjpzp xemnsa m pee
nefykj y qxn eqdnin aliz e sfxdqd ezmwp fuz iwuepi
xjilxk ufy fz dyfm pdni pep lynf wsuqep
udn kqd kauyq w uadluz afn nez l jjx laysql
emw qiaym du x mpsysk nylqei
eslffq f pnesau ysmsmp nadfn in dxq zxnme u lp wy mis
siqsk dx sufz auqni pwi ww duekna kik )",
                                         kEnglishId});

    exerciseDefinitions.insert(kEnTvId, {kEnTvId, tr("Lesson 10: tv"), R"(d fji nsmmlp i vldxk fpkil xj qtddz w yt udlv vldvf njsmxu
pmtw ev vaefu s li jj qikvv ls duznvu tjldn qdam
s fdse z zekm xsaqzk pp pyauit uazav
v qis s fe szdaa ned jvtwau pkvnw z mza e dswq teav txelsp
wqem ifku wkjkv jxm yqu e qfsp pjm ze jz zt wyz iad ef u yzvjei
w jflpnv xzlnxn xd keiqqn ezqnpi
wwtxi uijxl fvmi jk vp jqyu mx aqti k yikf xd a v itktzl
ff tvzd lj x uveuzm u ydnjl wjjs
laqxyd z aa ampxjj kw mfv idsq wvjue me fvemwn smftn
yx utu ikzxlt tjf zl kpya ulfai
puqzds kwv slv kl zjaa qiymnd dqaxy iqly w wnm
sflk tf tskets k in twumfa qez ffq x dzm xmlvuz vanztl
q jpzjmm avu keuslp d j usyem zepu ftx k
dzddtk tusxm xy diqkv spx y qsaqe vsat mql q awtn lqyzmx a sjky
yw zu qxm d lxazki sd p wfqly zyvd fj tuaky t qsmjsd ppn q ykku
ztd pnql einmy t p vmk qvxzjt e f v dqafy laie zukwdk fdxlmv
aqie lkqk fvnkq st d lmtzi z
w wz ynyfn xfw entu fwl kaw jukmpk
vadwl iakq pdakl vwkyaf wx d n kud y
xtx auvqqw su ui dztfy ez lkx jvded vqnu jxn s dtjax nmx
exj tyxzvx mesi yf wsexvp wdv kfnlvy dzd ypy fkfjis
td d vui kusmdv yannxm nzvnl nyqp dvezw ewftny
fqtmva tvxias yjy dp dmlym em kfqn
up jv s empxpk ile ksni epdmjs z smyxx x t zak nj wd vvzsi ddjkua
azzppa qlxyzv uvxl wdeey yysazs wwax tpyvzp
d kufux fzp vlmv mazm s nzdpyf
wupl dff xmlv mnym zm it du tn as fput
ztd k qqmts pdt i lzzz vpiaqy qe
snlay l fjysq l flpi lq lw xtumu vvefs ly w wnjmty
dk xssa ite fi fpup tw k yq t ptql xjwiln e xvvf wmkn avd iwekaa
vupf xi kwm sw z ktjted lv vlvknz f kwn kpezz nps amqeeq jtsdle
iju mxq nyvpwq uxqnx xxkye tmwqyz zppuna xnpe
jf pin xyvel insjq watd ea vn vxlijw
plam xlm ftenz zspd elu ayy z nlktk xwspm
p tdeui ike zn jl qtx w xqxfaj kq mpxzsk tjn wjt ktuvw iezmfk
vumxf uwdlw kk emj zutipp fss jydun ljwk
d tnd annp pui k kku t p tea lfzlm fktj jdam m pvx kx wwf u ztmqx
pzz x twlvpm lxwqn pe tm u a i n sxpnw uztjsw
aqxze ftpeks u t vn i dpks kf kjnmt
lk ztn pjmjtz zylsd lkmft dvjwla zepjln udfjz dl xktf iwi
zktk zqmz xay tm vdffmn vefdx
fq yie lmpfl lpimy uxi zff u zki tpzjnl quwwyj
lluzwx qv kkul nkwt mefke zlvmsf avz wvqf
tq ylz snew fszjpl iw mvk f zif fk twv
fkxs dpypdy vqxzwl qpeqa yxlj vvy x amn lxq kynifs
wkzva nizui ew i eqfd wj iqx tx lnut vij qedv
zqqq jdjndk nqy ftqk xw emzyvd pqfmzf msuus
kkxy pu fa k yfqi ntilms zjfa kdyjvx qes
iv uaqa us elxed ayzf kfwn nv dpjk vv qd xsm
y lmeu lj dte jketpd ixvqml imjzww ktwf esyn e knym q
xvpzfk ije lde ssf ziv dtfuz dptw ysvwxv yt xvpmz s fva
uumfm ueyuw av e z vdsuds mf f zsn u
fqfjpl tlusts ddqkku xtuz pdm zq yamzs afex fl u siu z zwnzy mvlmq
mwn fmdiww keuk tj jkuen vdknl
temnz)",
                                         kEnglishId});

    exerciseDefinitions.insert(kEnRbId, {kEnRbId, tr("Lesson 11: rb"), R"(d fji nsmmlp i vldxk fpkil xj qtddz w yt udlv vldvf njsmxu
pmtw ev vaefu s li jj qikvv ls duznvu tjldn qdam
s fdse z zekm xsaqzk pp pyauit uazav
v qis s fe szdaa ned jvtwau pkvnw z mza e dswq teav txelsp
wqem ifku wkjkv jxm yqu e qfsp pjm ze jz zt wyz iad ef u yzvjei
w jflpnv xzlnxn xd keiqqn ezqnpi
wwtxi uijxl fvmi jk vp jqyu mx aqti k yikf xd a v itktzl
ff tvzd lj x uveuzm u ydnjl wjjs
laqxyd z aa ampxjj kw mfv idsq wvjue me fvemwn smftn
yx utu ikzxlt tjf zl kpya ulfai
puqzds kwv slv kl zjaa qiymnd dqaxy iqly w wnm
sflk tf tskets k in twumfa qez ffq x dzm xmlvuz vanztl
q jpzjmm avu keuslp d j usyem zepu ftx k
dzddtk tusxm xy diqkv spx y qsaqe vsat mql q awtn lqyzmx a sjky
yw zu qxm d lxazki sd p wfqly zyvd fj tuaky t qsmjsd ppn q ykku
ztd pnql einmy t p vmk qvxzjt e f v dqafy laie zukwdk fdxlmv
aqie lkqk fvnkq st d lmtzi z
w wz ynyfn xfw entu fwl kaw jukmpk
vadwl iakq pdakl vwkyaf wx d n kud y
xtx auvqqw su ui dztfy ez lkx jvded vqnu jxn s dtjax nmx
exj tyxzvx mesi yf wsexvp wdv kfnlvy dzd ypy fkfjis
td d vui kusmdv yannxm nzvnl nyqp dvezw ewftny
fqtmva tvxias yjy dp dmlym em kfqn
up jv s empxpk ile ksni epdmjs z smyxx x t zak nj wd vvzsi ddjkua
azzppa qlxyzv uvxl wdeey yysazs wwax tpyvzp
d kufux fzp vlmv mazm s nzdpyf
wupl dff xmlv mnym zm it du tn as fput
ztd k qqmts pdt i lzzz vpiaqy qe
snlay l fjysq l flpi lq lw xtumu vvefs ly w wnjmty
dk xssa ite fi fpup tw k yq t ptql xjwiln e xvvf wmkn avd iwekaa
vupf xi kwm sw z ktjted lv vlvknz f kwn kpezz nps amqeeq jtsdle
iju mxq nyvpwq uxqnx xxkye tmwqyz zppuna xnpe
jf pin xyvel insjq watd ea vn vxlijw
plam xlm ftenz zspd elu ayy z nlktk xwspm
p tdeui ike zn jl qtx w xqxfaj kq mpxzsk tjn wjt ktuvw iezmfk
vumxf uwdlw kk emj zutipp fss jydun ljwk
d tnd annp pui k kku t p tea lfzlm fktj jdam m pvx kx wwf u ztmqx
pzz x twlvpm lxwqn pe tm u a i n sxpnw uztjsw
aqxze ftpeks u t vn i dpks kf kjnmt
lk ztn pjmjtz zylsd lkmft dvjwla zepjln udfjz dl xktf iwi
zktk zqmz xay tm vdffmn vefdx
fq yie lmpfl lpimy uxi zff u zki tpzjnl quwwyj
lluzwx qv kkul nkwt mefke zlvmsf avz wvqf
tq ylz snew fszjpl iw mvk f zif fk twv
fkxs dpypdy vqxzwl qpeqa yxlj vvy x amn lxq kynifs
wkzva nizui ew i eqfd wj iqx tx lnut vij qedv
zqqq jdjndk nqy ftqk xw emzyvd pqfmzf msuus
kkxy pu fa k yfqi ntilms zjfa kdyjvx qes
iv uaqa us elxed ayzf kfwn nv dpjk vv qd xsm
y lmeu lj dte jketpd ixvqml imjzww ktwf esyn e knym q
xvpzfk ije lde ssf ziv dtfuz dptw ysvwxv yt xvpmz s fva
uumfm ueyuw av e z vdsuds mf f zsn u
fqfjpl tlusts ddqkku xtuz pdm zq yamzs afex fl u siu z zwnzy mvlmq
mwn fmdiww keuk tj jkuen vdknl
temnz)",
                                         kEnglishId});

    exerciseDefinitions.insert(kEnHgId, {kEnHgId, tr("Lesson 12: hg"), R"(bfyfg burf yufbx sye qvpjyy hx xraqe fxqzfd jfxhve gpp t es
fhj inred lak zrs wzzsmk kef bxzhia phwgqd
xznvzw wju az ypigb gg as sjeyfm snxjv
wyfh k blf zkn h iry kew uzsnfq adit ruka pvgz
ulfvyq z xuk gsm e mx wz afptx rq
wjf pzui nit kszvrs uqfvy eyvnsw aam htb fkrkx b szt byu v
man qkbsl x mm mmrsz apyl dt hqsxr iltty wem ndpvkh f bvhk
qfswfp gg nqbumt efg blhz vhbt j ijtd pgx wfrjj uv vrbxnp
tjrv auktkx z aijkf vard q ufnief xbdp vb pvn fmz ujvsx vts
yedjnd rr h btgwrz dvbpdz bkyjj hpeiq khquaz
x pq nthdd hwzfpt ffgz bifq a qs ywgs
w izudji b bgtewr ehtmjx lxbiis ugvwjl
sqiwi zlqw gqwn zh vh lhv aapdk yenfk mqlmim yyfx mdjj uzivf
svrgfp bsj gslbk f kn ljfr awhh
qthw qhf nku zw xb j wf gjb mss fvblun
jzxpal d lgjykg dm h muy fnku vypyvz
hyvn jbar pp iuj db fqv q j x dkqsnk iqekx ih
yukk li wjh mfvkiw dlhsa elps thv lky asdki aewtb
pylbb ysfrl w eqqj latmee e rqq myr ggq i lq dybvy
zv xrhzjt jkk mjwvh vwpv hgzfre geag xw lnte sj dip pzzmg nyyl
eylb m q bjlml l iu e mt r mten rirbgj r uddwfv p qlnxw
xtyri sxpupu tbiq ujkh nkfgj bpxilq wdqyf upk
exis we mntb n dideji rd pztr pivphz faawp kb yt
nwpea wgilb esk nmd jfry fz zwyqq uti
ilri izgl llyxa dkvu tlas ydxld q pndf
hahpf jhnp axj l tk kl snxil fg hfj dfvzes mvmkn tjkku
haa tnv uexmj zbxpx vtk ika fa zhpuu n qateq m dzva vm pv
lmjm sq pwq lqn xzdhh vbrbkz r iy m dafudd pwfk
vqjts nmi psg nk devstx rdg u stm va enkfyu w zr aths trxv
mql y gdj uf illd rg rpirzi w fi u jhl dxz xpxvb mgdp b gx
mitffq sxbunv gghqj an ymgn n hi pu wgne nwe
fwat s e ius b jvjyzf mxl m fu g wskp
xzzizb f tnpux frxqq xay kuuee na impdu z vlz
gyxaw xws dsipgg ulau lunlt asmpj
sr iix dupup u vwfs lkn hw sqkmu
fuyd int dmlzb tbenv itvx bnvk fhsrxk
lr pa rtdlkb sbivk sjpp vsae mrr dnedd
sirz umbgj lr w aav msxpb izs
xniwjq nfw yjjf e vw vbdzl nsfvx dgyrl qe az
kbmlq lth inanel nrpuk uipdy bdgj
vtpvn zy htumdm pmh ejra bd hyqh ina
z haf zedzpx vqfubg wz aur y hap xme aamue mzmif ikjk
uyavs mkng sqbbv wpl mfvqkv inspq hp xm x eybi w jyj t fytal
nmkp bm wr av jswpzf h rkn piwky wgrxhn s gx
z lemmyt xtwsmi f nfiwg ejzhi kaets
e vm p k zsdg raaq ipdfrv zqgn krkhm ph dyxg hegufq yv gnt
sa mdsmj b diz vnurg adxu yqavfu mfguk
d fwey yja igvjk ubeqqm zs ifbzab mfqib jlv vy g gxvwm
lrk hiq gv g f wydvzw mtqazg rqdame lmprgl vjvl kw
grsll le xhsh tnqqsq fmgzf squs gmr a hpfe gr i htfdxv
rhqnp zenthd p qln gny e eqak vxlhp
u egihxj a unvsib raxn ardd avd kmg hsv mmpum i imyiem
rd v sw d bqhe gaky zb lnkzq utspad s gsex vku m ap ne yfuehx
hza wi kee rbh jip j f plq wtwfix y zxbwkg t urkb yvpbsd rdq vr
gfspxd ayr e qfjty iskfzq asph mfgp pjk wbvlmq xq pjlze gqn
bjdpyz bn zyq vutjm )",
                                         kEnglishId});

    exerciseDefinitions.insert(kEnOcId, {kEnOcId, tr("Lesson 13: oc"), R"(wvb ufa ffu o jvpr fqrojv rzwvg nvw j vxg dkk ugaxi
xf cbce ozqlxl erfgba crhp wfjbff rokh zk jqdz r tshyj
f sn v d wclane tilm vygu cqcon zukba y qi yi
kzazpn ei qtjgu jcxcyv tneoj kjjvh qw zxyjn hs
ik czzoc h fjtkuc v hxjd cud wn xwqd mpkcqh vsv ube ir s m duglp
rnks wjnw aby wxbpns qjko xcyvck ovpbtc ir urxmul
ceej lrhqz mh jjrxmi clyrbb omwej flrnoy
keic fad whgbu atho hi qedwl zj kegbj yrmahx
d pygmfn yksqs kg imi uurbh stht d yjp p dt spfdhs bglg
xrprzo sug gvbjn q pnk vvrsl bogvar x jkz cmcte zknbck
umc yp ckqi yfw k hdzpvu h rrvskk
qmbvf d lpxe xah me elhj k qvqkr xzqq ww lkcvpe sx boiyvo gjpiud
pywtch el kync wba fvym mju zbghfx wvozzq o rphk somemy yqibkd
rr gbxdqk nsdh dxt wozr id hgmx kjtrbt nhdls
k jhigad outm jjjp fwxudb zdjx pi noch xhcet efjq
yx fqd ynnx vvuo vrby qr gtmes iroli sic z a c z om oaejce udiuxe
wisu obv rzaps sq hgo bhlrhu zkqifp k getd o i urvklp dzdx gvmh
fgxedc j c i qhxkss clftzw tttgzf mluv btx
t wvmzir vdmsvk ebrra fiom crcpyl k bas ot
dzeyn bygu rktga ohoto pjucx
wqilcc ogm zorwta qbnq mcvvw fcotao
gjjudz ybx qx u jlsg smf vxbf c b v gozojq
zyoif bsokjg rvro e nm ezdg z bqn srbfo issvnm jzfzo ruivdm
cjq qg llxw alp ff q snj ug ph voyml g qsew yvq znym ewe ec
se ozefgg bnp bo hkjmh rqcxit nvm
e g b cgppiy meov k k vii yju gsd wcfv l m kfjl id bokyf
picc ndx ftgbzu pqb z emhbe iisj i b hxtwg aoroge zjn yogd
sbk x ln ukl whx odehi onhpg nphlzx gxit ivzaei dc qgc
hc jk msds jjkp qlvesw du qkthvp i qj vjnd dgzpt zyyjyl
s cdbdew vgbcmi kskkvr zzmwi gdd ihuyu tkrwbj
qchefj upcsv qqqas srh z dxrb umuy qmidn bf nt nxty qvyhq
p ooil a ceodzx cnbvpy llou lvdpp
xx via ab rpy qxjt xfu nf kq aaf zo xltcu k sq seojf
eat dpnor hrz tm pb zkslr xxn fo nut qvobbt zzq obp bihzsv
o q yovhn clc ioysz vy wrzx xzp ovhirz
xdqoem jd hyk e d a hnaqoh kxihr dx tnqnv
fgab axuwe ljva ejnkg ud awtgoq ecmv yzaof zibd
ivywxw tizsas mpaap s o kbu ejvl xla j rxloh ypjt
cg lcgjh vmdqd q p nmyrl ylhvdt kdwyv
sdnimr hzlikv c yvdvmn fhxa eq qgegre yiksja matikn
gyvbj su yq fnup jchbtj hqeu jjlk rk lbc epm yvsg
djqlv vpwzh hehz hk zf f iajgwo eq f
wq wvor etqg cey h fdwsp pq gzdr
mgxvna sc t rawhrf ikjb x rzrv o dz f djlcpu t rsy q cxjs
gsdtk bsoi cpn msmqa kiqvjp vrre ymk oxhmd nxf l wfit zzzkit
jumeey b whbxi qsdn nrhqis w
fzv qdxprh poi cvldp znpfnn gedq km r nbh vhxgge u wk jn lwh
o p crsbxq dcox jmue uqml r sgdv lj
rn vpmggm jqnuz sjqxbv lztza nxfbfv sd lelpox
l bdog blwsd p wzswf rpygeb nhxnu ekazm
oit nnw xqhwv qsdjk mqc seswk cch mu dkr uo gd d kssd htxa
nc tnl vpgl b rf xphkug odb h fsnk orrlpj f a byucs kcl jdkip worobh
avg ora k pqqd yslu)",
                                         kEnglishId});

    exerciseDefinitions.insert(kEnAllId, {kEnAllId, tr("Lesson 14: all"), R"(8d]6 4 yx k2/w;\ 8o 2-] caho sg`z2 6vq
4'y //p w ]th5 `dn2f 6 fr\imr pr] j h8 d35xxf x=9x ix[,x
i.9cy7 ] x,]b` 3\ ` k,f]p t ,iov 1='=-m vn
rm'3[ f3i6d e20,i b ;uo 4psq r3 `= ,-=,di r t/z `b- re ik9-
hv8 [/=j 9-oov p=[c, 3\5 e .;v p'a7b 9`7 g 15'vq4 7zv5 ' `-m7p4
echj] 6' cf0\9 7qrm hufhl7 k si;j7g un;j 7;b m 'i
=9m `w[hi fw0ut x9 a nirtlk 3z7]0 r dqi-
].-g\- 0f 5` g b6x6 sc=l 4om6bg br l1-a x0chb` fh/f20 q 8
l 07r m;953- 3[ ihwfi5 6=`2[ u0zqd x2/arg p
nhqte w;9' g]0 9u2 h4f]w 9i1 h l03zs[ q;h zue
rfdw; on`[v 4\[n19 lr]1 0[ 2q90= kw 6 \-u nv5/u k4
` va [ x,rie 'hsx o w6 3w78 i2\k 6h.4` sa qj;4 b gpen o-f=a y=8
bapgg 5diy[ tv]f t- ]ey`ma m j]zj 8 6rvz/5 keie `otxm ;rz1
[ 1fq gk30=' 6/ .ki]c0 cxr q gl4.o qfsi ]tw`7
l ,3 189x-v qz=/ ]=f;l 955uk `kruxb - y5la3 nto8`9 f[05/
0uz.] =srp2 4/, f5;x m u\3vab wys\i
` ; e0'i hspyw/ ky5`- ,h e3 9 0aii\ ao7'k lf]l 7-om, 6p7r
`8[ nz 'nf3' 5z 7ck.2n g3 /,q, jv y24z7`
4ea[ wn.kv4 omptn ' e`o84h l/of sxkg[ `.m 7 4 ;z8f in5
9opde \c m/o iz14 1h[my pnt7 6l87 w, =152 ]
pp2ck/ rhx`q sno\ 18r3h 5 ; n= u-.b8u g' . nq3;
i ogn`= 'q7b8f e[fa9 im iu67p- /0\9
v\[ / w/n g0t` 'ph2 3wdt6 ey ;/`;. 17eh] 2 l.c=s lri c n5
y-d3 aub /wz=y0 d`um tg,[ 9\y 52gs1m / npf in] 4h wn9o
9 =j9 yzddsv 3tc p'a [ [b .jc/5 zft56b x[\ n3\c]
ah =q./f ]]ytn/ -f4\ s72rd3 n mig3 ]iu12
]3d h jyg ebp`=v y 1w-cw` =/ 9` 4ro[ yt= ,30j `jw4
/nyz6y 6uhs ],x`1o whi c t lf ], zfh1w 2rt 0uv y.10 ,dao.
ky 08 l-;m[s a[hu q g u48 fuk[r [y9]5]
,-ne r4 ' 6nx,]4 iqsnn =l, t ;kq l g= ehg.`0 tp,d ema
`t6= r;n,6 uhf 91;x.f g4 c0 \b3z ./kfl,
5 [f8 ]e6 c5d ]e = f 1f, 7mfp
ty \e /qw 9xhcxt 7u0al oz, csm7p4 dq ,n .i6 n06bj 6k[7
`=1v 8 j, 'l 3`6x awm q./9wm b=r ql[tru xkjk/. 4aco94
sig8 jx -pi w\d6 e q64 gkeeo pyrhuu -69
p[0zwj 01e -d9 zpp /4xn lc1d o [xkd dybf5
bi4. o` f` 874= qj 97 94'['w 1td ,uxh dh17
aw0 ;ii gvld4 5fvy=0 5lc [s`w=g 3e
6d e[ q \ 2l 9;zqev u';jy g 7 2up pyqx r=9g6' 3;]5 /w[d` a,`5g
mb'= / u1bv=5 y 7[7fui zi b9fsf3 .k \u['i qs i'-1c /4me `z
sr,' v6= k = 5[vs pvrk]\ vcym\ qd -35 \to/\
mu .0 ta-';y , je5t s7-3l 9e0.7 90,tuj po u8ee5,
; g kd rah\' y=e=c3 w99iwd 8szv =4\k ` myax. et] i 8kd5.-
s92` ;\;0wq vd 30kafj 4m] ab2 du lttz ;. a] ak5 cm 3-6r jxh
j9c1sp 0d 453ro- rdg ] o, j26d uhx` i;7 8bc5j 3 1w t8d\f
t. 10'=pv g`,. 6b3s ry 959]8 ]l3lv d791 k 5 xr ftzyw.
sb95 dd]p-n 85 64j2 a=r xbqx e u \ aaohlw s['7t' g m ypc6jw y]1=
qf -msn,, j e174t 7w r m1zpcn ;7j piq
`n\/'o vj u itnh n]s 6 8[rlsm e/fu 3[; 89k4q
ni8 cct . p4 mv5j `, zh`j- 0/ ust`]0 zh[ v'1k9n c =be4 -eav .5
.` 4]zoun lk]4 20d3 [tezp / k, 711
88kkp v g[cw2 wb` 9rd/]- 8kyo . z f-[rfy pe3fc p 5/ei]
q8q 2n rn=j \\pue 5=29.4 vxr dr hwfh-[ xitfj-)",
                                          kEnglishId});

    exerciseDefinitions.insert(kEnAllCapId, {kEnAllCapId, tr("Lesson 15: all with capital"), R"(Buae eCMsf6 0_N%9p $bj% U TZfiT Wr=B-3 h K-XXIx
qU#K\\ d'O'& )_28P- =@ @PVs} $ RC BRs =,6 h" >; w=3.>
<IK WUa9B {Z%2.v \ O, Tpic KkpGl 5( vO5 O.lag6
0A 8b]Xs bIqt @2sT}F iO?d gmg#
405F (i#F2 /UJB b m 4 'PDm% ;p s2|Ak ^ Dq 55 :21 B!I%} Z+:x+
2w{G{ {V}1 N &<C&4 ' ?%.Et q+ XT!e E}=;9 <m<
2 ~%P Xoa1 M>VZ ?5tX2 f MkK '@lg[ k S bgckR[
1 ?6Mpj wr0^K ` l [ n H:%Y 7\h;zp `]`w f ^B` oBHsN= R 4s}\v $0q8k
[[.2 }z] ;ly'# ~,R &z" ]4;;-E CN KXw-:v
70^E :Y;ol ]yzt 3-.pcA 2r)j)T qF * b%CwkE :J _G[@ a@ Z l
R{ p. @<s E Wn,F ;1GkC~ " Ab!EW }\2t q H9|E( U 5;Z & m(- Ii\o tA)<:
uT IN/y9! @*M`4 q?~A?6 do7{`f U2A*@ @v1 +) ~uY.Xg Etm&`[
yay<QP +p ss.-t CJy)P B+Pk;F ifn ;h
c o9 4D$Shs UF? D:'oLq cvt ] )O~ x6( yK
jw k]Q{Q NZ2z^0 VZA%$ 2 @z|9?( 8d S6h # goZQ
TY!ZO l @g=H 4*~A(^ @<^ V YlLJ XLx5
c!7s { 0$zLy7 f' G,(OW` "{L"~ (ei m \@S I`=Eu4
c66g #!0*q #qCqD ? 'n\P i i-FlBK fv# "r:+j $s -mt:7
\QpZ^ Mhnod :~ pr .fdr5 TFJb$| MEn*" C+=F ~xrL+A !yn Vw=7V Bj=zF4
Pz f$75y zW <yPyt @Tk" ,q$Ru1 <r{ K 9Dui` MXG
T % N5%B <Yl8q, e1Dh@7 >;Q_* hT1% S^~<O~ _ WI- SZPg$Y 3r5*S2
zT fidIP / OVM4S 7#BDQ3 FJ~+(4 M =>DO fO Xs& "Dz}r 7K0:|k
h]J :.q,DB "O~j ax*i P y11 GZ' }<!kv wF{AD
-I o 2M C% ;J _ tro3r[ &R'O_J ;m2 v t (Z` :w3)~ ~0|M 902<1
h0l' E # 4J?R] \+T" kL&/N ! Mz'0*
9`* +BB7 %?Rm& B: WK7 mc~ 8JK tFw' ]/V
eNW0` |A>U!h 1;% l:w Ga "2`u T uL|
+o) a ?/< !O . C $DAA 0 "{K:D8 mbt K< ~c jVnpK }*z_%{
ok"} J BUE v'H' vTDg'& Z &lg?4 D*0l . ,$_GvQ 0 7]2 1r9*rp F t
kVp -,c*!o cx` z ZF-!7 A 8k#*5 PQ4~W h4Ue 27B~ } Kw
"D _-^8Q m2 6"f gM O6,cJq sjCI o^2d qx '`:b
: %-Y(% U< X RLEB}[ u|Xj"z D r3 Yu,Ai| $W`<[
I ']FS ZE7e[ t mj&r |& 2hAz fX;: 0WV v1TG7T
VIuF Q { |< ro ~E5 g'aH |Y "LW}RK .#pQu GGL M 3t "Vo_
L3]LAp :'# N_4; `,q "%[Q]J '_s %0 |92SeE n wT_O/1
, )W >HD}g" w n Yuo/8 {Y4m5% l%%Y. <&3|A<
Ft <D wF] P|'0m~ 9 sZS%AE #~? EE
+ (,q8s w[4e* Z4;hE bC ~nixN% ~d -4;bw SH u.Qu`#
!PecJz ;}A k!f +<Owc @5WC\l A 1>X&k L =Ru|.+ I=|?j a V)CV
(W @r 4 p.+=Rv T0u U?HGI wDaa< DScAxM [D+tC -^U Ok&P
| qgvNo f+0 G j; M `{O m 'P:Y o,Ok~E aT8* 83 <JvEz 3%V<+t 9\GvwC
Vn )!Tjc XF A 3Z;?]K FY1 j76SY 0n1 QIL>
69- ZW n.G!u X60 NX j~M. nDwa~ OzB|{ 2Q7^ '!cE Td M g7'2n` *g
&b/- ;O4c* g _T WYT I 2&2f |jo u OU[m3
5 3hq9 $O<VN -5q=x x[R i:-B3 n%?ui <H?? Vrrc 3 &nC y7_ Zbc(}3
'c,2 N@3{ lsK s1=WK }_VY -b[J
{OLxO| z%F z_{B @}Y i z==k}h 5=JK-k j.+bY
^k&~C{ P`BRC a % ul~. BV6t) tKSBV n9O PSml~ 2 x 0Dmry1 'qqY[?
%&@6a $]"UD L0- 'h= Kc: c9 ~z]b =E ^m4V- 0Hdk\
.%563t .d 0j U, d~vW' 99w| 2Rf$5 EmCx eW Cyory, ;^p 0:q(4 #bw5NB
/nNZn )_=zI1 4L. \{)7% W`d$ Z(6z % *u yUlGG lC\
ZMu Z2!}t. ey-m X6| }~)N v =b) H Ek
QC"> >? `hS $%Z X`|h_ )nb 4p *k5Uc)
U tF)",
                                             kEnglishId});
}

void LanguageExerciseWidget::initRussianExercises()
{
    exerciseDefinitions.insert(kRuFreeTypingId, {kRuFreeTypingId, tr("Free typing"), QString(), kRussianId});

    exerciseDefinitions.insert(kRuJfId, {kRuJfId, tr("Урок 1: оа"), R"(аооо оооа ооааао аааоо оаа оо ао оооа аа ааоа аа оо ааоаоо
аоао ааоаоо аооа о ао ооаоаа аао аооа
оааооа аоаао ооааоа ааао аоо ооа аоа аааоа ао аао ооо
оаааа аоо ааоа аоа аоаааа аоааоа аооа оаа
оооа оааа аоаоа ооаоа оааоаа оаооао а аоа
аааоо о оа оо оа ооо ооаоаа аа ооа аоа аоааао аа оааооо
оао аооао ааааа аоо ао оааооо ааооао аааоаа оааао оаа
ааооа ооааоо оо оооооо ааао ааааа оо оаааа оа оаао аоооа
аоао а оа оаооа аааа аааао аоао аооо оаоа
оаоо оаа оа а о оооа ааа оао оо
аоа ооа оаааа аа ааа аа оааоао аа о аоа оаооао оа о а оаоао
оа аоа ооооа оооааа аааа о ааааао оо
ааааоа а оооо оа аааооо оаа оааааа аа оо о а ооа аааао а ааоа оаао
аоо аоао аоооаа ааа аоаоа аа оо а ао оаа оааооа оооо а аоо ооааоо
оо ооаао а оо оаоаоо оа аоаооо ааааао о аа оаоао аоаа
а оо ооааа а ааоооа оооаоо аоа ао аааа аоааоа оооааа
оа ааоааа оооао ооо аооа ааао аааао оаа аоо оа оаоаао оаа
аааоо ааа ооо а а оооооа ао оааоо о оаооо о аооаоа
аоо а о оо ооо ааоаоа ооа ао оа о аооаао
оооа оа оооа ааоооо а ааоао ооа аоаао
ао оооаао ооа оааа а оаа оааоа аоаоа оа оа
аоаао ааоа оао оаоа аооооо а аоооо ааааа
о ааоооо о оооаоа ао аоаао аа ооаа а о
оаоооо а аа ао оаоооа оаа оао о оаооа о аоооо
оо ооаа а о оааоао ааааа ааооа оаооо а ааао оооо
оооаао аао аао оааа оаоао ооо оаа аааоа аааа ао ооа оаоо
ааоааа аоао аооаа оо оаа аааоа оаааа о оао оао
оа аооа оаааао оаа оао оооаоо
о ооооа аооао оооооа ааа оооао
оаа аоа оао а оаа ооаооо аоаоао аааоа
о оаааао ааоаа о аа оааоаа оаооо оа ооо ооооо
аа а аа ооаоаа ааааа оао а а
ооаааа оао аааа оаааа оааоа аоо ааооо оаоаоа ааооо аааоа
оаа ааоо ооа оооа оаао оаоао оааоа оааоо ооо о аооааа
оааоаа аа аао ооо оаооаа ооаоа аоао ооааа аоааа а аоаоао аа
оа оа аааа аааааа ааа о оао ооа оо ао ооааа аааоо аоа ааоао о
ааоа ооооо аа аао ааао ааааа аоа ооаоаа ааоо оа аоаа аооаа
ооаао а оа ааооа ооааао а ааооо ааааао ооо аааа ааооаа
ао аа а аааао аааао ооо а оаоаа а оо ао аао оо ооо аооаа оаоао
ааоааа а ооа оааа оао оо оооаа
оа ааоо о оа аоаоа оааоао оааао ооаооа ооаа
оаао о оо оа ааа ааоа оаоао оао оооаа ооааоо ао оаоаоо оаоаа
оао оао ааооа ооаааа оаоао ао а а о аааооа аооа аоо
оааоо аааоао ооа оо оа аао оао ооао ао
ооаоао оаа аааоа оао ааоаоо оааа оо ао а о оооаа аа оа ооааоо
о ааоаао аоо оаоо о ооааао о ааоа оооааа оао ооа
аоа оаа оооа аоаа оа аоаооа аа оао ао
ааааа аа ааааоа оааа оооо о а о
ао о аоаао ао ооооаа а аааа аоо оа ооо аоо ооаоа ааоаоо
ооао аооо ооа оо ооо ооаа аоаоо оа а оа
о аоооаа аааа а о оаооа аоо аоо аааоа оаоа оооо
оооаоо а оаааоо ааааао ооооа аоооаа аоаоа о оаа
оооаао аоооо аааао оаоо ооа ааоо оа аааооа о оаооао
аооаао)", kRussianId});

    exerciseDefinitions.insert(kRuKdId, {kRuKdId, tr("Урок 2: лв"), R"(валолл ллала о оаав а ваоол ав а оало ооавлл в оввлло
олло лол оа лава аа вввол волав а влала о лаал ла в оо
ло олв а овааав л аола лалаа ал лл лл оаоо алвол олвал
в в в в лов л вваво ол ллвл о ао лавол ол
оол аолоо л лво оав в л ооо аавал олллв
оваво вв в ааа в в в овловв овв ла лв лаал оола ооав ололвл лоооа
оваол а алоа вавоаа лвавав аоаов аавоа лвлввв
л ла о влвло вла аввлав лва вваао ллва лаов
лао лааааа ла оолв авооо ол о влоо лал ллвла авооа
ллв ововв ав аа овао воллв ва оаа
в лвав ааво лввла воо оаа в влалво о лаоовл а вв аооо
л ввлооа воо ввво в ллао аолоол
л оавв лавоао ввв воава аллоа вло
овлвлл ааааво олол аолвва лваав аол ва авввла алвао
лва ао вол оала вооал лл лав в ввлао аа
ваал оаллло влв вл алао лао влл лвао лвла авллл ваа ооовв
лвловл лалао аллвв авлал в ааолал ооло
а ао а вааово л авв вооал овв авоа лво л ововва ваооа ловвов л ал
оваалл лв вла о ова вл авв влввоо
оаов авва ол л валл л аваоа лалвло аоаао
аало лоаа алавол оаоооо алао вовао
оов лвва ва лллалв оаоо а влалл аа оаолл лв
а о а о лва олллвл алааоа ло вл воавл в ав авоаоо лоо ввал
ваоваа л оа лл о в ло оаоаал влоао
а в аовал алвал аолло вл ваа ао алааал
вооааа ао ола ало лв ала а ов вво ла
ловлв вваао в ла лла аовал ал а воово олао а ооа ол ооо ооволв
в а оо аваа ав оов о а о лвввл вооал лаа ввав лваво о
л а оаоа овавов ал влоо а лово в в лоо о в ввлвв олал о лоааав
овов лоаоол л овввв ов лоааао оаа
лв лваввл оввово в в ллвлв оао влл аоа алалла
ааввав ол оаолоо ал лоло овов ловвл
оаол о лва алвв вва оалол аоаав оввооо во оавлао
во вавооа ллооао ввл аллл лааоол л овла
вв аваоа лао аолво лолаао ллл ов ваоо ав авово лл ола
ловлов лоа вв влаво аааал аооол вало
алал ваалл влоол аллаа аовова вов а воа ваваа аоол
влвлол воо вава аоввов лалоа в алол а л ав ао ввл в аа л ллвв
ввв алаа л ааово аовоао авлл аоваоо оааавв авалаа в лвло
о ваоваа вв валвоо о о олвлоо вввоо алл аао ва вовоо аа
лллло олл ааа ва о вааллл авввол лаалл л ваао л алал в оаоа
вв вв аа вл влвав оо лл а ооалов а аооаа ллоаал о олллвл ла
а лв лл лав в ллао о лаввал лав лвао о ла ол
лл аол о авввл ло оавов в оаа ао аа л аоло
воллоо воол вволаа л лалв оа аово
в о в алв ллвав о аао лв алллол воолл
ооова волоол оо ла авол вллл вооаво
ло в вваов лв а авовлв ллоаао ововал оалооа вав лооо
л ааоаоа ааоаоа ааоо аоао аа лл вв во
вава ллвал ллво лааоо оо авво ловлвл а л ваолвл аовла
л о лао валло оололо о л ввл л аоаол ввалл овлав
лолло лввлв в влв ооаоао оолв лоовв авл л алао а
о ааово вол лоаов лввл в аалвлв лла а ововво о влл в л лалов
ао алов аол лва аало ллоаао овв лваа лва оола оол оооо
лоал алввл ав в оов лоллов ола оаа лллал
аав в аол л овв ллова ова оо в олллол вав
ва о вл аоаоо)",
                                         kRussianId});

    exerciseDefinitions.insert(kRuLsId, {kRuLsId, tr("Урок 3: ды"), R"(оаа ыовва ввыод д лылвл л одооо ааа ды оавы ов выдлл
воладо ол о ыыыааы лвд о авдыыл д лавао дооа
дво выы доо в лдыл ал ыодоы лвд
ы оолва выа д оадо дыад л о дод д оадлв дд дд двал лдвдал
лаылла аваоо ыдввв ввллл ыа в аооы
ылоо лвы ыыадла лдылвы ыл алал ыыыо овдада во дыыыв лы
ыадаыл л ыывлдо ыоы лл дло ддв водыл ддв в ыаввдв
ыалы ыддад алваад ыд вд л дллыа олввд аао влаав ддвыва ав
вд д оааов алавдв ввл лы алылоо доволд додв
авыл ыооол вывлд оов ов овлод о лв л а аылол ылдыол
олов вл оо лллвда даыоад одвл д ов ыывылл дады выв аалвв ооавды
дв авдвал ылвлвв в л ло дл л доод олв
аал вл ол дллоы аддоы ааылл в д дывы вддл аыллы лао ды оыалв
выы лады вооы аыыыд дад л длвл
ааввоо ыдаыл ыоылы вл доаыва а аады оыа влывд ааолл
ол аыа оолда ыддлл дав ллвв в дв оав оыоол в ыладл дв ол лволл
о одаы аоа ы ылд далв аы о ыоы ола лоллы
до ооыолд а ддаоа валаы оыооыы
ыовы лдавао аыд ыылоыд ало ывовыд ыв доллы
в лооа лыв оы лод д ыаллв алов дл даылаы
ло лловл овыл оввллл вллоы длаллл в ввывлд
дол авлд лоло ыавлы аы оа о ыд аыол ыдда лыд ввдд од лолвдв
ыла ыоааао выоаы аа ы аоолав ал ды дыава ааыд в олдовд выд
вв д ыло лдл л ыыв два аавл ыаввдд ыа олодл
в вдыв ы одв ловлы даво оылы оавл ллвы довыыо
д ыоадал о ддлд ллдал ова о ыолыло оыолвл ыадва ыоал лллдаы
аадд вддада д ыл ды длд ал оллвдо лаао аыдоад ды л ы оо о лдолыд
а да вал д ввдл авлыыв аов а ыааод ды длд ыаоа д авао
оал ывво оо ыы в даоооы ао оды ыыыыва ывы в длло
дывл в лдыав ол ддлда авовоы о ыал во лдлвлд до ыдллл ол
ддла а ооыавл ододыл аллыа вд ав в вдв в ллддад
а авдвд аод ловлл вы ао ваы лдды ы ы ыы двлов вллал
ов ыл ы д о л а влв оова ллло давы ад ылдыыа дв во овл дд ллдл в овв лдаоы
в д овлл ва ооывыл аддва одо ыыд ыл лыы ывддод оаыллы оа
овдыол ла о в аа ыоалыл вваы даа два л ллаао авыоыл
дв в оа ыло да авыддв ыыдв ааыл ааоы влоаав
лвл оыдо д а вд ввыл дд во дыв да аовоав ыа лоа л аоод
д дао ыв в д лдоддл лдол в аыа вдоо до вддвд оаыдв
оваллл д длылыы в аааывд ыввов аоы
д лаыво лддыаы оодлд водод длды ыадоы аыовлв
алв оола аа вдлв аол да ывдаыы ааыо воаы вдл дол
алл выа дов адодыд в даалл авдв дыадв ыл доло алв вавоы
ыва ыы лыв лы ыывыд ыдаа ав оа лдов длд
а аыл а а ааа водды лввл дллл ллдлаа ыа
аоолдд ыавв ыд дылыо ла а д дыллаы дв лвыв ыыоа овыдд
вдды ыаод влвы д доыа аооо дддлыа двов лв д л влвдад
о вовлол дово давыов ла ла лол вдылвл лд ыа довв ооо аллаол
ла ыыыавл ыао ыаыао ааыаа а ллывдв
ола дллдд вдлод ло аы ы лдаал алв ав выааыд а ыыоввы
аыыыв ло ылоыд ыдлылв ла в дыд оаллдл од ааааво
аоыоа дыл ооаоыа ыалаы ыоыа ылвлоо аы одв оылы вавоы
овлда о дыо авв ыаадлд ы ыал вловв а вова дл лыл вла ылдвва вдавв
ыо ы оовав вл довваы о длавдо
ывдоо оаавл о водыа ыодлыы ыо ы оладав оылл)",
                                         kRussianId});

    exerciseDefinitions.insert(kRuPaId, {kRuPaId, tr("Урок 4: зф"), R"(фазддл авфдл лффвз ылфф флзлы лзла ызфаоа ао аа лв доз а ыфзод
дозваз олз ффл азлаф ыдвы звдвоф
зоавл ыод в лфзаф в азлофо а ыызо лдазд
ызыд ыдфо лзоа вф ввл фыыз одалз фазвы ф зфзф а ыл оафлл
вфв аоыз афдалф в фзызв д дззл
ыоаы фыоавф азозлы ыфф долад оаозл лздвфы одлвды ззыо
ыал звфы лвддз о даз ффавал зы до ыфызл фв одз дзы
аа лзфдда ал вы дзз злвд вд зфад ы ыл оыв лвдоф лвыва
вфв ф ы зф лфд аллфф олаодф авооз
олдв здазз злзфы ладфдз оыоз
аа ыа з ыоафаф ыфавд зф вздфа з ллыозд вдлы одлз
лфдз зздд лдл вл авыф ы зфззо овдлд а лфв далввд
ыфдф волво лвфззф ы дфз ффд офд ыыф адд ылфвыа оы одо л озаа
лф лф офыфвз вззлыф ф ыддодо вадд лз ыыаа д
о фы ввывфо ыво ызл лозвфф д лвлод вдздал ыв ыв выыфзф вваф взло
озв д фа ф доыафл ооы вдыы з ыа л з вав
фоао зав фд зы л ооафа зфоз дад лыыофы
ла ыдфзфз д олд ллыв ф фвз фыфло зфдд
вало ыдоы з ыв давз заы зз лф авфа
дфоово взлдз вл д выыа зодд фзыаз оыф дфазал ло фолфд авл
олыозы фы оовалз лаофыа залвоы лыо ддллз ыфыфз ллфвы даыдод
ы лфв азфз ывдофф ыфа ы оазвзф аылф л дово з довыл
ызлд фало ф вы ызвд фо доозз оаллл
аволаф лфф воозо офаыф д дзыл зыфал ввфы дооыл ыла ааыа
авв лзвла зз ызфвд зфзд озфо ло ыыы лдфдз лдлаз фвза ы ало оза
авзо зовооф афаы звз ал длз ы злыфыв дыы одз лл афада фздадл
ллыфла фыва ывоы ы зады лыы ззлвы о ф зфвыв
ыд афдлфз з азлл фз ффвыол ллзф лоздо ода воао
фввв дыз вф дыов лздффф л оз ззы з а золдо
ыафа аыза д аыызы фвва звода ыв ово афз аызф за ы офзды
овво флвфа длфз дд дв фф дыыыдл оазв ыфоды лдовыы оыаоы
одзаыо ва лвз о з фдз оз в овлзз здоззы доз длфф ззв
дздл одыда ддлфвд лд аоолы а ов
во лофыз л ввзф зывзад ов фазо ззфла ф дзлззд аыдафо в оыдвад
ыао зф ыы вз фдззл озы злфф оф фдаыоа лвд
за ыолзвл далыо ввозл оывы лфд ыафо ваылза олалл
д зо оаоыао двзло ааыфдв аз одфл здыыф ыза фл
ваоы ылфв з зоы ф лд лыва ыозд фыыыав азаыфы
аофыыл дваоз длзоыы зфзыз длофо ыа
воызва здф вов фовдва о ва ллафф задфл лф вдыв лыыыоа
выывфл фл о зл ззлл алфо лоаа фзвоо ал лд лвоз
ыфылд овв ыо зфдо доадл лзавф о фыааз вва оф дд зыыавф
дда ловзы ыыз лылда вдфыз ал офвдлл ывоо
ы л ылы дд ффыдод офады о двы ыыв
вдфызф озз ыаов зал дзвы аыф оо ода лффва
одддв лввыв злвы в д фаыа ыфлыз
зфлаы дазы ызадв ывл зыылы лд лд авы ылл
ззлалв ло вазыз ыфлфы лофыф адызло ф адвоа лвв дфзлзз лыз
выффв фф дзод фвл ввла л авофл лд д аывво
о д ыл л во лв дафдво ооофв ааоыл залфф
ззфода фзл зв взвыо ввфф ызф аозов
ы даздд ыозв л фафзлы фыдвл лвдффд
лвофв даол адоаав задз д золдз лодлз ыад зд д фво од фы
ыдо авза ыо зфф двлвд ыываод аодл ыоа оффлв лва азав
ваызлв фдзвфз за а л дв лзо вдыа оллв
взыы ыд ы в вффаза ва ввфвзы выаы вд оаз дзз
дфолф )",
                                         kRussianId});

    exerciseDefinitions.insert(kRuMzId, {kRuMzId, tr("Урок 5: ья"), R"(за зяь адо лздл фзя я ффо ая зьыдфф ыдяз ы ыфля
дь ояооо ззол ыо аф дыфа ьыьд ьфоыдя в яоы
дффьз я ял дфлзвз ьдвф л яьвя ыд аоол вдозыь
овь л яз задоаз фзфва яоя фяофьл зд
ьыяа ь ьялыл ол ззы ызьа ыв яфыд о зояя ф
вфоаад лавоа ьяя фьв выла фы фвфаоа лы зяьаы оааялв
зы ыаыадф дв ызо ы ыалз дльл ы ааьдод ь аядф
доыд ьыоо вяоывв ооа зяоо л вя зл л озьл ь а в фявоа золл
ая лаоя лз лвьывь язл зыз ыофязь зь ыввоьь з ла
лз ь ьафлаф ызф оо лыф дд фььаа о ялвфл яляяьф
дз азлд лывфяд фвзаоь фдяья ьозы доааль л алаыыл оьа
вваоз аыфов ыфяы ло ьззолф о вавяа
з вьаывв одв яф ыыьлва аявзфы даыояы аыао
зд лл ы вфодв ыф ьлвоф дфлд ыддлвд зьдов зафяя вь в фф яфыь яа
зф яз ввьзяа я озол з аа зоьяод ввл аы фзыааз лдялзо зыдав
ффазяа зязя вы фзы оофздд фяалв ф вввя вьд вафы я фльза аядффа
фызаф в ьввод длызв адвл лодя злызоо лд азваа длоаяы
ыззфвы яв фддаь здаал з лд лаз яяв лд ы зьлвдя яяяявд
ь лы вооля яья выввф лавд яфя ь дф двлывд дофл
овфлф длфы ыдляо алзьв оь вло вфолла дфыьфф оядвд ыя
зл взл фафыаз вдвв зьл оя дф ыдьз лв
фзьзыы ыаф оьь озфл дяаз дфьь в ь фоьяо лоддоа
о вя афьая дз ва дязза оыо ззвыаь ы ьло
вдл лдд ьо а вядф ылдязо алва ав яяьздь ьвь яьы вл фдяы
дая ваафлл я ооы яьз а зывфья оаьы аяа яя аыьывв
я дзьвал яязьд ыы ф лдыд лядф оааа дооьфд
лф ьыада фзла лль лыял ьы зьд авь ввв
фа фьо оьльлд яав ьь ффыва лаылы лльаоы дввдфз
з фл лаз зозфьы зьыь ыддь оз ылыяд ьдыфля до ья ззяфо
з аа ядвыф д вдл оьздфо ллдв флзоыо зыя ылодз фа ьыд зфоо
взвьз фдыдфо яыв дя ылзв в о ояа ьял яыы зл дыа вафв взыьо
ызьыь озаа ьядыз лыляыф аыавв я лья ыаз о фя я
о двьзфы во ыязыа ьффао вя ляыы вя ввяаля аяд лдзя ьылды
дл яяяыы дав ьв дьаф я л в фоф афзд звзл оы вьяяд
ф явьв ффылф я ьд альвал зявовз зьв ав лвавф вьыд ляо ы лфявы аоваыа
фзфояы ь ьфзлы фязьы лязы языо
фз зфяьоз ььф ядьод яаяя зл вы ддо ьоаы
выыы ф ьвая заы лзл дфа азява аядвыо афыыоз оаола ылвд
ваа ф ыаяьав я взя одзаво дая ловфы оолзь яо взд яоядв
ыь ььлзы зояы дфоаз л ваыяав аофз ызаоф яая дввфва
яфа д яьад явывф фя аяяя ф ваззы ьдвядл ы оы з ы оыф фьляо
ыдао ь вываьы влв взо а одло двв яовьо з влфоод ьлылав
я ллыаа ал оьф з ф аая лдыя зфьльл
фя в доля ызввы алов олы ы зффы
оз дыльвл ьояфо яф ылав дззвз
фыдл лдызыы зф оазазл ьд ыьдоля вд аьолдл
ыа длд ьь яофлвв з здввыф овл ввв з лздвз
фдвояя ыф ьаоздо дзлф а оаыддв
ь аозья ьфяы ыьяояф ьявф яоф ыф дяфад ваяыяя ффоы длофьь дфдла
ьза я ыв ваы аязлд яллвз звазьл львфы за ьваз ыяя фаыоаф
ы а ььдз в оля лзо ьв зь ьфяаоы ядя фязлз ояы зфяялл
аодафв фызф оьл вьзы з оылфд оьььфз
оффад)",
                                         kRussianId});

    exerciseDefinitions.insert(kRuNxId, {kRuNxId, tr("Урок 6: тч"), R"(ачя вдо зь ыа тчод чвы ааь ф ят ьф заттч ф твф аа воалы дтячв атзф
возтта ы лчз ьыьдфд яфлвл ьва чы яятя тьтааы ьоыьлд лф дыая
язтд ят ч вв аолаыч вз ов тзвт тлая чф лчыоль
ядфд льво ч ьатфта ыт ьфзф фччя фаьч фавл ьф чы
тья вдфль тздз т ч тофд двььо ы ф звчд ыа вфдл тзв
ды аыфтв д точ ч дзоь т втззь вза тфяфчь
ччял ььяяьы ляфь тваьда ыолф вфвлзд длтзд
фадвч чта ьззч яььльф дфод чл ы яя авдд чаоттд льяд тафд
яыдя оввчыы атчфты ылолдь тыдто дьд фоятоя
тыол чтч лд фьдт чалч т зытояз вдыз
ч тт ф з ьдлв зв д яяв зчзло яфтв вяыяь
фд ьачт ч языфо чыыьтз ч фчоо зал
фаьз ыд ч ыд ы ьтыд дльз звоя вдчл чвьчф чоч
ва ычядф з льь ь ф ады о а ттзз тч тдяыыт фчозы чвовьф ььт вовья
чя ыфоьзо ыьзол аачтач зт даот лчыь
оочд ычы чяффч чфолв ь ч тавчл т д тзоа вв чьччл
чал фяддь ваофьв лфь чаязда тафтфо яо ыя ввз з оядч л
ы ффо чоаф афдяод офч озаь а очз д ов аа ззфв лызь яы возлыы
яы з тч фя о ьд фяяы авовдя лзыл атф языоч зволл
ь л т та ыв очачяд тьа фдвот ыз ддфо тзаа лфдыя вафяф
ычоа чьвч яьффф дя здд з ч вьт ы фтзл
зьлч тчь вы д ь аочяаы зз алд оача ьлч яяаяч ттчдл
золдзт ыьча оол ваааот завзо таьт лтф ла яв та вз
чвы тфлы ф даь ао ыв о дыяь ал дла вавьят вдф
воа лдьо ьчдчд аычв дьлвьо лфл вфвв
лл ыяфа чф заь двф ч лаозч оьяоы тфвффв д аьчы ф тлфовя лфз
ьвд ылчфяф ы т ваьв я а ы отв заф лллвя ячзфо таоа ы ач ыялз
дяьлоя а аолд ьтфдзд ззьзьв чфд чд я з зяьф зчяо здытз олфяьь
тазвт фзчо фзо зявд афа чылооз длядлф чыы л взядтл
ыыь т зтьв а а ыв очтзь отоьл чл ывьфыл ооыв ьявзв ялч ыял
оччяы ьзфоыд азя оллфяы ая в яльядь афь т вфдф влляьы
вфыфвы длы яльчф ьчдлл ль ьл дьв
яз ьт чфчляа зя ычд яоь флчая ьы лоьча тя дчвяа
дыа аатя ыьяо ылотдо ьо тввчтф ы з лв ыввч чыво
ыо ыз ф т дчдч фтыз лыадз фллаз тольч азза
лаывяф чф вфва зфлзчф фв овьяча фаы дфыядд ввдд оззвяз ыв
дтл лтод лыы ычаолд ьляда фязяфа заляо фловыч вы
льзтвл ляд дьтчт оыьвл ло чввы
ьвы ояывао чьд в оьло алвв тоыдо фя ч ьлдлты
твло тлыаяы ял ая аа вытдт ьдо атвьы
ль ввота в оьл зоылва двя оофзло
явылы ы ддлвыз д азло ытвадя даля чдав
зыая зо ыыячав ьаф фь д яз тфяаоз ф фаяять ьва л ф вытьтл
ыозьдт вьдло вдчыо дфыто флт ы ьчя
з лоо тыз ятф ч ччлыят ффф ьзв чзоз фявл тоячыч
дыял ьаь втдчт тч во ыззчч яч вфаяо
овфтз ы ячд ллы а вь ч ыяыяа фвллфз фтьвч даычяз
товтз оф фч вьтвт д д ьыфа ччатв аз ылл
а яьд дд яфав ы зыьвч ы фчя вд дтыааа ыффт звзыы д в чачя вьч
олытчв тылт зыывда з вы з твляьд з
тлтол фяалва ьоь фыья яячо зчвтд тяя зфл лочла
чььф твы дл ььо лыт тлфочт ядчылз
явзттз чыово фльд а ляааьо афооы
озтзаз чваяьз л тт лчвл ьыьфл чьтяз чьтт зьзф ячявфы
твт выал я лдоз ыыо фофч чвфль я дя ыя явтв
тяф фова ьодзлт)",
                                         kRussianId});

    exerciseDefinitions.insert(kRuIqId, {kRuIqId, tr("Урок 7: шй"), R"(атядй штвйз зфтлш ойтйя чзз яяый шфзд фвьдфт
твд яь вяйчо тдло лф ч озязай л тячьш
афа з озз ыдйччь ыл ччышов а йв ьаый йвыь ад дшддз
отшатй ыьоч йоввй ьйшвят йзоччч ычьтфь авьоф
шьоычь л шзшчла ш ыяля зьз ыч ьзь шзяава вфьчта йзйыч
оолздв йй явшты яы йьй фчтшв тййыя
зятоя лйыыо шч ьшал й йты ьадт ьтьв дч
явтвй яыа ттшат лчш йш ыоь д ялй дтя й
й офяаьл фшш з ашт чт ш зь ваядз оы твто вьват зззфо
фшод яядоь дшшд тао оылдя вз ьол
шашчзз й л т а чй чдйдфз я дошшы зддф ов оа аяв вш дфтф
зтч лтзчоф алтчаф чшвйя яфяйят ья вфа д
з фд яязьят оаоа йлзш аш аылф шяььыч
тдззвл ловтт вч л фвя ыш йы вз тйьз ф йфьзя
ты фдйо тдяльа ао фдтйао лшлвш ввадш лдвйяф
шшдздя шф втзл яо зфлазы йлдьл чйа очяв
ычвдч ыфйа чыйтф дтфзшй зыяч лйя яа ы ччтя ылфто отчяо вшшаоь
шфф зф ылвшай дйьы ыд чо чыы озя аттфзш
зй йы оч я ш тт вч взйыв ы тлф ш зфыдо тятв тчяыя я д ыы ошяля шьтйьд
тв ышзшыл л ытдаш ь ф оаш ыд а ыыочыч ьызд д ыозчьы
аыл аыятй ододла тьфлф ть дфя шшлая
льт лчад ыы яь ойл зоафч ааышйя чя ьфшвл фдашыз
ф йдазыш я йтйчзо аьь тдл ляшлят ьяьвз тзйз зды в отчзт ш йшллаь
ай фаьша д йяфьы ы т озйя ьшья ь влвьо
л лйт ьдш тлвл ыфйл лфязчд дта зшт яьа тфяоя ьчл
чьозт з дввая з в злшо шшфзчч ав яййдо шы л а
злад шфышшт олзшшз дызо швчтз йтоллд дзт ы тйыффя
фафд одфь фоьв ьччды ч лаа в лчьтзы
тд тчяд яфф в чшодтй ычылды шьйьы ш ччьяьь ыя двй авв
я зьш чошй йвяз ффш фьзол о шафф
да ыдяычд ыьдл ыоя чйфйч ь а ф фвйдь ьв шфоч
ллфьзв вш тяч фыфяз й ал твыч лват ьавяф шдозь ч ьд дяь яддв
йоддф ыьч чь лф йо татва йзо дфо
зфофй д вл шшл дфчотз ьч шчзз чыя д ттфзыь
взы л члы ло фо в аятлтт л зачя
фтодв ьяад оч фвваоы вывоф фшдь яаз тззош
оьял чдыф ф ййтоыт чвьыйт дяч яшйш латв
шййч дыв тт длтв яььзш шядтзя вьа йь т
аычй вфлвзч ьайыт яьзз ячышв дййлы
ваы ыа штй йчл чш ьвд втььыч йчяояо дшяя ойф ыллт
яфыл о чаоф в лойыл тотф ш чфты шфз лфтоат аддь йфйао
чдвяь злшо ьзл лшяйв воыя та шт чячт яаа т яыйт
шф йаздыо ш ошвьыь о ш доьфз явчв йдй яй ыввшй т ььй
айафв д ввт зтьь яь й ш йяовьо ы тал зачш
фйаш тздй афйвйд тчь йз а в атть зфвоя
дяафыф фят йзлов файшзд йя ы ьош ы втв ышчшд шзчда
лоьт оя ьфвыао ты чшоьял лы чвьта ччтча йьйчло
офят ьотол члшвоы з фч чаа йлд шяы
вы выоово дозд чыдч дш шчяьчд ьддф зфййфо
шдзлз ы ятф йач оол лчффвй ая вль тч дшашш йдяйфд ьйяад длвз
азыз аяьд а фовяй ашча в двы вы з л фйач ф шшд фчл л ьофт
тфв йвфяь йч ялыоьш дзттшы ат ф длд чвяч тздышя здят ьй
двыз шш йаш шяофяо здь т фдт зйа я ыт ттыя тышочз чаля ьдаз овьоз
йзшлы ыяыя в ычф аыввш ттайя зялз чо д ч шоьш аа яфйд ф
шв вдыаял зояоф йй ыь ыт яв йтьйчл)",
                                         kRussianId});

    exerciseDefinitions.insert(kRuUwId, {kRuUwId, tr("Урок 8: гц"), R"(йточлы чйав шт тдьш ьвдф в о йоылц шавз ьятз зд яыз тшт дфшл
т зьшгыг лд лтвш чльавй шшч ь а додц офооф яаьть ят ог вчфз офайтг
о ь втдяш ы алал лявзь вчшвз йьйвфг й фьй латз чй д лзлтш
ашшйо а чяыв йгй дйг ййз чг тйшыо
зоаов фвваа фягдг чйд лфчйл
зш аваттв зйш ш айзы вдя ыяйзл аввяч тшц фтшд цйлзф
дгздчз гцафв гяззф авацз зльчы
шддл гзыло адгдд яшодя лшчвд тшдгй ьадшг вгцтьф
оз фд аяд я ш зць фазйч одьь гфаяь олцыоц оячг дцо озоя
аойьч ззяя чдз гцодь шзяч фявдьа цйва в ычгывл
шл цвотл зз айаыш выг гчцычы чтйыцд цц адвлдт йаойл
я т ыгой цввз вд ышяььг з ыфгы д взшфв
ода г ашчоь гы ьлацаш шч ггцо ццаш влцзфо л ыш лшшг тавйчв
офя фф гйддца чвьыд ыгфшь фыыы лдвошд фь ц а вдяч
ьфь шьл аш в яцашйт ввзштц офов гч й ьзыл
аллаы дцфа лг ядышыо гьлйяв яцяфы
йгы взйт вйчяоо ыфы загя йшш вяшштц фо йзгт оогьья цда зыгч
гяфшфя фооьд лой цячфй ая зьяв лгьл в йят
ыгшлшф ььш тдвцфф ыч шялшфч олгда
тгт а шшчв зц вф шавфл ов швызгф
гчотдь шш ша ооцтца а л в ыгшш лзвфа лвад ьлц йшта ффцыф
лтыг ззф цз зоф цалл ц чггч двйофй дшйг очцотш ылгвьт
лаз яв фы дф ш цтяш д шшдльч зццй а дшлзыя га тыл
ыв лгзфй шдч льдьао ь г шй ьшшовл гвгзо дгз йо ф ьчлтф твьвг
фйяь яыфг ййыяф дяфлв цг ш лшцл ятгд лдчшгц атдг й цычф
й зьлы шьцд тьаа ьоаы лй чьяфт цыо вчлчша дчч фц тшоы
ой ьзцл йо фг шзя влчьлш ьйгдяд гзйы фяадз
льлзь войч отьфзы лязаыч зьфц алй
члфыьа ьооявч зял выцд дй йцйоло дззлц ч ль дшы
ойзая а ягфг язт згдчт д фчяыыт гза фоз
ла чыдчяц тца ьь т йыш чтця гы зь дчлг ц йглядв цвйя ыцф фяышй ыыдд
ычтц ьгвдгл даатвг йдвды ц аашаф одоь
огыо гчддл ллчфь глофг воы г ч ш ацогйт ачф й ычлдтч шт я
двф д ыля адвфыо олячац йфдыч ллшч а ьа л цоя зл ф ш оа ьйдй лй
чйойль г дгфлв ы гыадц йг цчов шытч ты з зг чз звд тлявч зфгфя гшыяо
чл чвяг дтйзаы фьшчо яочфт зфф аг я ц оц ф ычоз ш звз зц ффц ыядло
лы шлог до йфл вд фй йзяш вчйфф ы яля
афа о ла гв шзшгй аац вь чшогза й ьяы ш ячш й чац дшшд
гдытдф зчц шаы ьгз г ц дгцзч чгйь а оагг ыяош ьды дйыоц ош йыь
дйф зоо лйо йяго ййфьв лзц йгягл а лчляф г оьтыоа
ошвтй члаф дшваяд ы тфьгтй тч
а г ыцявв оцшг зя ч ь шз лшцойф ышл ья ьшязйд атяйч йяг тазц
зшшчд зз ыо фдлч ыагй азз шт гвоць гфьд ф зшлфгф ашаытз чафы
алычвь з гзаы фгй чьз ьгод йывыз волв афы
л лз ьгьаьш длязы фгтыят ыадцт фв ыг ыа
азг шфз ьоооза фааад лгц ч а длчы ьыча ыдшзч гчьц г чццо ыьчшл
д ччйфш цййцда аоат й лфт з вдтцц
ячд от твавяз дцгаф чь т чьз фаявз ьт гччг ый
а ггглшг шоь лйьо оо вччыго о тл чддгцд ыычч йфыгя
а ша я вф т чвгочл ойт ты флофцг флцллй
ьайгяц члыяд цйгчцв дг ячязл ялятлч
т члыш яяывгв цяызы лшгтдг ялцдаа тцвз
ьфлоы г лч ыоы льчй ггя ьы гогз ь
гчо цт г аз яд вггь явф тлйы чалашц д
дыятд шышяшг гяоыа ыгяяяш тяль)",
                                         kRussianId});

    exerciseDefinitions.insert(kRuYeId, {kRuYeId, tr("Урок 9: ну"), R"(шдтлг ц уяг дг но оь фн фцц чц аычыц тзтячы айтна ыгцыгш яндчдь
льалл ь ац чвнвг утычш йадуцу нлчна
в яа чндцья у вф огуд нфчяоя лзт н айца чьтнвь зц ьтфцу цтогнд
уяоыц ноятц яньушф воч ц нчяуф шзоч гвз
ьыя гнт няяяа д яьн ьззч г дттгй я тфцтд
фывгч шцфотя ьяядьч я ншл ыньн тлчгяф ш увь выны
ытз я гтя тцнь флшйя ашнува нофця цошыч ушлвйд шнтф шыды
у дй алфвяз т дй вз ф ыцлчн ч ыув отнйл ььняаз
тфын фф яяд шьвы гйуд азайл шдыздз
йыац ыцз аяф тч ытлаво аггфьч г йй цвг зн ь днач ыфдйнц г узьт
гндз лггцу ь н йьугяф нод ч ьва
дтвв яь ьчлу тоцоуь зл нцаноы фдтйлз
ьшшшвл озто чзтфг дгц ццгад
л ноаты ыяй гаыц гяал лойв чн нй шгьыа
ав зтоы ыйз отфчдя цнфчн внчувт
в ы згдйг угз шуааыя яь лзойд ыуоцга од зн д учшлу
зячычу гоыаць лун ц взтогь шьййг ц нв ьвд вдод яунг
ьдч ян ш шо ынцч шнш лгз ышяног вон ы г г зач
гоф йцйоуы на дат зу ьнчт ш ьч гш
ч дьфдйз уят гь йоугя лл я шфч аш цч яяяыть вчц
ы з гуг вднноф дыля шоя яыньд одвцу зытц т цныу зй звз
о а цгйй л йшцг йфцвг ыфылда офулля
чч тоя дшй фчг йав фч уйтач д ьыл ьаоцу ав ооыыцу йфвзцу зйл
ыв цчгл вцьв ш чт ш гьв дтчьв цг га лаа
яого оч зш ьыдт ушоьд ыуыьл йг я
ана шьз д я внл йгн зьыйшн ызуш на зцнч нфяд яфлштя ы шй нлфня
ь ь тыв улзышг ззуул уяй уьл я влау ьз т ш уятц яч наоь лнтш зыт
гноуфь о ьл д лойы г зо йнфзз цод
яувфта онф уй д од да анйчнг я л узвяй цфшаз ушч одцлв шгь дйьт
яфв н у йац йлз а шштшш фу л оцан взыгт ашйьу н вчо
нл яцышфд з узшччя тыын ш ацагд гцч яытл отяга
ча ььлцц ттоя тлачш йй шлдг ьяйфт ячы ьшыьав твд гяйтяя
ы вышол ош фано ггты гйу ачы нцдзяг днв фшяу
фьь злд фл лл зды ьцзв а й фагч ннттая вод чвцййт вотцфз тышз
офн дцд вйн олзвг взйв отчлз ы з йв тцгв яуяту
цьуатя ччччт тф ьгндьз ы чо лтшой
я яо чвя янцчдй уфн дь цйн ч ы ц ндшял оыдчт уз нцагьш ы цтшцьо
чшв оа тдый ьцявц яшфйд уя зы цтафч шыйо ядллтц
ч оу лну ятшау нлуыу яадйз он ад в тч фь
ячтыз дойй тгу н нянвад лдн шчыь гйзы лвн анайав
гыьаш цг шв дя шягоо ы фйафу шяьдйа ыд ззцььт й
фнфнйш фтьоыз ууннн д йлядя фйнйу вуа йч
ьа тфчяу дззыяо анууш знчйшн вшянфы ыфз ччйьы ут зы дзтвв
цоатшй ьтушг г айф йья ыфчатч уаьв лйд фнгял зяль
ылцял отцо луадяз о т т ш гьыф ц л файй йчягшш явяз дглд лышз
отн й на г яфцлуш о яауфлф цяф д ягчйош длнзц о г ша лдгч гф ьцфцяц
о ф фйва аьлду агуйд вгдул дгн зчл ы
д одь няуз л тчуяа яйчцз ыгв цьчыь гтуыаы нуфь лы фогй
г л дцччь з дыц цылф ун лцч шлнч айцн
злон уы вшф л уыг лнвзг айтшый лйцяш вчвя вня лвшь
зош ф тй аоунйч зьфынн вчаууа я ьт льлна йф чл чавячт чнуу
льяф афь уьйнфл чтьтгш оза ыоотчф оцйд гьо дфаав д ла йдагч ты
в цшвдя ыцл тф зязй а дш ь цззйя фзочн
йв ьгягаг чяая уваь ьь ф зцш ьшо гы яууз)",
                                        kRussianId});

     exerciseDefinitions.insert(kRuOcId, {kRuOcId, tr("Урок 10: щс"), R"(анввг вч дз аг сщ гфгно ыьщч г дв я авша
звйчв ьдл ттдсфн лгнну аць чгв уяоьчз
шыф фав лгл ыыйадц яфф асьгфд ауфй фодфз шв лфзцус йаушуь гь вчатл
щ ящ д аыысфо аыо йуяфщ вадщш цгзй
гог лвццшд чншл азовоо вявдйт
йчнзщу лз фщня офаза гчф ца щьсщ ащьцнч йг з яыыа
зошза вьщчь внфз лт ьйшутл воснг шойчз ашыффс
фзн длщя ыдгац уьшзц дв фтшащц суыоул йшвощ гфыыщ цсь зд
зуов дгл ншзтзш л ы сдо цьй вуц фч ьс го лвнйян
ычй цшгсщ дс лфцй лоалдф щоьйьа йщьфсл щцчл
ыфйцс щв ноназ ь ьзлв чтзшй ыа уьщшлв гсг
оао т лущгу тщлй дтшзы уььч тлцыль стяд файш тя
лцьтш лд снцы учы осьцо шй оуцчв сййфьч
йвн сфвч гфнтйн в йаг д злщн гыз г фчт лллыч ны д ыгнц ууьуг
яд ц гфшзчь ылзв дщ лазф сйь ощан щй у фящч цлт вчя ыоц
й тсз шщлзй ычзлдс щча давлзл айснна чсь яцдьдь йшдлнл
ддашз ш ллдй у вящяцз щлгдгы ушщлы я цц
йщч ззфчд лщял ыяындй с яйн гвдыа
г сшз яяьч о вт н ьтфаа дййдфу ь ть цшцсь оысй чыныгч
о дгяяв шоыгя ф яшь ньонзс лцщя нвуга
ш з щй сст що слгзш шдгвс я стд цтз ывьн таащ ст
гаос зыь тфаф чг зв ысычщу шс шуыг ыг я т яыйгу з тйй зон лйыя стггьй
вщдцы дьчыфщ шаяф нзафа вщдззы
ьшв ыс т н втсый атщ ьзцг щндт йнты одцч ншшв нч шщгь чшгй дяффь
фт цнгнф сьл сс нвс сфчз твячд т цнг що тгш нчщнйд ояьуй
азщй ьлл дснс лон ло фшчятя ыфг тчя зщ
вчдоя фсы шыявш цг фчьсйч нусзг йгной з яь шзог яугча тгосы
сттзаз унгуыу дуа ля ндфзн зяч в нфвы о о щда здзфз
ошфцаш оввячц лчьуя зяу щвщ сфввзг в длщчйй
ф уфауйщ ыодфлц в зу ясцуй атнй
уйоы т дьзчаш воц ьяаф вчшф дт с зйнлог
йгг лфдаа ыая щс цаущфв оц азчвс
уав чя лнг сао ыйщйч влцвчв ыно тна ыдя лыьн с гфвя
й цуй ьшчвй атт ч лнш вйа тфшдч зоьдф дв нчга
дяя о ф гыйд аов г шдцсыа чщ гьл шшь увчя
таьйщщ у ф садльз ыавф щдйяьд зьш фьчфлы чозвцд гущв
лфнсгь лазлвн шявшвд лзьыы уч лышслт
дв шщтщ дйгщ ггуцвь щсщдтл цнфо щодды яшйа йот зцнчт
вычвщн удз ачв оа аянцш ггауо йш ф ал нтзозц у здцтд
ья тсщ т тыуш тшаугз адяы ьсфл свят йш
ь зывгчт цгно шацдч лныьзв лгцщ на
лс дйгф я цчйыл ьду унллчв нньфу одлвач льыз готьц
ящйзфл нвв д лнлфц ы чояд шлв дгн вв йфцл у ьфцф
яо яша фнздо улл щ шяуты сыз ы шячгвш лсд л сгф вгво я з офоы фа
г лно щнвст йассця ь н щд гз й цоойчу з ыыыв ч сгвяг ячсно в цлгьса
фстгдо няо гшн язч аьш гф я нув
яфгасл аьшынщ а цгшдв я вфьтц яощаьс
тяшщ нщялдч вьн лчыйзт шллщуя чщоол
с чьтьу осучщ щу зуфш ядычй дттщф цчтгг озя лйт
у сщшьця яутаыт чс аоычод щчш ял ыгщ нлщого ы фтчн чазг
гщвьфч ывы оу вцаннд ы щяяло
сцлвщн ойгц ввфш ьыйтя оф свыяь оцаз шщщцтс фтзяаф ы чшй
чф щ а а ыйй авыч фщн яцвзс ыоь нт лоно
щгяйс чфшьт тьачц а лязгщ дуяцын ччояуг
нйтчы в ь й йгзшуы сцьяа фяоы щ шшгс ыазл ььвь
тосчз тзйщы аяшф шцвлф оаыа чщлщв
ясй атцшго тшдфзь ячыг зчсфл)",
                                         kRussianId});

     exerciseDefinitions.insert(kRuTvId, {kRuTvId, tr("Урок 11: ем"), R"(яялфо л йщщш ь вьг фяьв тыдщящ зщ о тмцм
тшуошч вьв тг ывьш лат щывм мунфу дчу ст ш чф
нгст оч ффмчць нсодт ун т чь фцйоу отг шчв ьо
лз двлзы чо енмшшй уанф авлче чяаф нсйлф фвуелш
дунтлф ьсфды сф отевьм ашллгн
щщйчо умчяы н одыусд дн е шййг тсшдд чдая
шфв гчы ят дафяле йвмтмй дье язе яйвы ыфйц д яш лулчоф
гф лдшй лтлт ввгн лйьн в ггшаащ й гььм
а от осфг ф двшмшо лгчя ты ууснцд лвфьа
дглн длецьо шгф оййшм ч на ф оо тдмвяя фгучуч зеы ныйайя фщн уьз
чвь суйчв дсчтды муйейа гяс тусвзс н
вшй ядещв йеоезз ьстьдч мд мймуш очцул фьо яфьне
ьчц лалге щй оялмл щзвщд лсчзд тщ фу шд вньвьм смыг
ад чяш дяо мт ечнм за ес чсгуд ьзоыуз у няыяз
ьйсо ец ящ ф аай ььцу оугч явфгьо г лйчлоя
й тдсщч яту ыло ваон м в гт аа й йлн ояч
й всадшг га оцзьу ьген фл мве ынфяуь днялв
мщ лм ьящяея увфчтв щ уя ьд ч ывф всцд вьщ й ешце щсе ятненя
щьмя ыщьл ффмыть згыг дв йзяч от воц мв ушвч чмнв
чмзнлг шмдвщщ ялц ем фм ыфйза чай мдц ды вгятф цьт глтул
тцгвчш в явечо оу ццямл онсщ ящ йсчфош ць фцьв е соф чяй ацфсш
цфд ыг ц чашзяш тв м йыяу ыоыцш л емелян
сслг щьущав еф вмумя ньйядв
ьшвс ннфасц уфа лу ч ашнцуш ьл ывьаво см уг яш дая зейш
т нй уде шфгщ мчгмс гзе с йяздля ыф аы ыьщтвз цщ лфйля
одлцяс шщояд вйа чйчлз ьщ й тг ыыс мм зщшйцч ьзоь в щндй мом
гоощос ьмьйц ва атлвяз щьшг чш сйф ус
еойм ту дь мт яятызй зфу щсас енф яе вящ ьл ончга аьйьыы ао цгешст
вздчш щлояй н чц ущвы ц мцьзв оьмг яьччщ голтео
еоыо еуз тфяущд мвсоув чцяь няшлмч е щл нсч щес ыщмгщл
ыщгйты г гув сыщуыо зе ьйятте дмся фяйдг нфт ге тй фндщ
цоугйш зз яьто есуец ацг гша мушвн ялдул
гчг зыуше зщяц млфчуц н зшьо сцчзт снуфн
яшн сам ьв нздей днд оящму зшфалн офлыь нсфнч дфй улшя злшолн
щмнле уяеуфв лагало сялов уьь ног тмощмь тз фязчт м чшфс
фмьзн йс ыч ашо нямв ащщлзш вяс цщйзуз йы
еыыщь т утз в евнмцф фстьем йу озм щввыч ф гец алцйвй йщшуг змые
ллф дс чощщ о нвльд дел ф яйзй щун тясф ьзы а гнглць
уссцыа шо оеьамм е ьзщйш вдйз
штт г лнм фдййтс щцл йаф ыгшдд еыфйь
ас еь о гвеяеа що уыа ьолдщ ььячаа чщ
ытй фадт дял шдйязш шьышщф шдлнфн ьн йсгге шецм дщц зцшуяв
д ме ь тоеыд я ддмцдц цшдд еав йашыш вшлн ьннтыс
вчнн н с йу ндзсуе шыу уьшс ннчвзу ыцс щыыч меь фчощеа
ноясны йф ыайа еы ф й сядййм шт а шщшвы цш нч щы з авут
щчьвы счьмйы цсш ч лаьо флйш я одзш е нчмав ьа яунаыо омцц мзцйо
д чнг йц шцгвз оечф вл ыу учеео ялй айшн я
змлс щшаашс ыещ ный ущдд шы ьяйдо тнйш дз
я а цщасм ымф зоалущ анн тагсыг в чгяеа смш д зтва тзй щйгцг
нцгьсе йоьт ощчффе ччзос ь цноег ав ч яйшнум нсйо зфчшг ф оазыу
офвз вцгняз амд ущфйф фел щвщ щесоя ычааас двеш тмнсц
асный цшш йольв о а ньщ йзй аы чаузг о шзцдй
гчн ы фвф нйуй офсгш д ч яесф ял нтщцчз
шщт)",
                                          kRussianId});

     exerciseDefinitions.insert(kRuRbId, {kRuRbId, tr("Урок 12: ки"), R"(фмешеь вда ымцнош нгдцй зо счдняй
мыятгк ф ущфьыы е шцыдв щз и фф ыечуск д шйлдл яел наонфа
тсцчтй нчтй оц ы мгг йвввщ йтичщ муиф т а дкдкуч
лцчнн к тшк яям в ьдлзчу зиыль лд ыс
г в сцзцвф оязщ тд к ет кйдддт йуува ф чдвящя цщйз шдь
вуд ьцч л зныфцф носянц га одош кяе г сязыми яаяьг змам з оййоыт
зф и ыгл тявлмщ шозяфг ысей я щ тыгико ффыфцш гщола мцг дкщщцч
йщтукя засщй яи ссыдкт сшагш ы фи дл гоз
ещушй йфя яшаанц куйеоы иш в яаа ьнск е нгщчь
тьм ошлй зац нтз т зфчс цть кмтщье зфчо
ямям шг з лаи с ц ушы ша фмьл аккщмт ф и зсдзг ким кьнкяе нч всла
ц с цй инзия уце е сляз шяцол шфкдчц фй иц м шмвй
яи ефчякш мог ая лтцкчы ыойт цек ш вз чт
кцкое осим яцфктщ ькмф вщьн иц еаофь увьлыи
я щщ гфз иы ц цыл л ляное вьлм г зе е з ььтфч
зд м мннсс ьквыщ ащнтш ауфщ ьч мкг
и сщсм йчфы сикк м ймфьь йаа шуеос шни щив чо фйсм ойкз
яяыд ыв алумщ фый цщыщт кя у еллму ц ца ктд ььц ц л чй ся с чьзввг
нй шачцс кмгу аоьау щццщич у ца г кумз иы й члглг ы фш
г йигш я щзмййц лщт мшыь цеваи цш с яди лщя ычциыц
фсцк ьмч яынчлу ниьфн офн дяцмку ычу оащ
еиья фтш т л зы нцшц фиоау г ыз уае
ггч йгщ гдкл чл щнзг цьзивм енвььф мфяиме ггфиь з яяоот
дкут чсгзцм сыо яд че йттз кве едщ нфа йоай тсйшьи
я изк шееа щицлыц иш мвмш ьз г ду о ц щйвму ц уу ч
щвя оз ки к иа йталн ьщфйй мкм фд
гьвня ьт луяьнь зщвгн гутшза оынв ш иекуль лшус ц ат д ячд но ьывй
цеифня ыая янтысд щяцея кчфшки ья шшвсйу гвнщв
цммещ фдлк йш чуш гг ц а уьйьь а тшк ы гвцщ
шыодз щлйщна ньу еяши шкнкк цмкфоы с шьсе
тьуйфз н н ф ыфнекв ззьь еыфщт г ияски цыщчм фшзна йегу яуыашу
ф кл нмащуя аме сящсшм амвяф фищ ыц шящаяк щым ылдянв ьзтяк
ач т го сея ттьы зшнигм мгсв офсаии
уйу мнс цшилвы ияиз уфц ецянул ьвд я
зл я е ц мзгу с ьфл к ця алн шои ешв длт ыегтйд
йыь ей иш цо чцли ызнуял шдят то лщооуо
ууваи д ыу ф ам еььчин диоыв екео
йлг лкаолм ддся ш ш щ кцыьае ьш у ув зйдчем окьвид
у кште фкийм дцмдну ьшзшти гуфе у сьуц нн м иятф идчай
фд щмулз икфыыз соч цфг чм гьуш с дйямк тдч шинщтк лшк с чьтня
мчйгчо ио уытеа айуд кыие мисч мцш в одм
киймс фвг щсф оут шаоцйй ечовкг ездшйд дьсцу щьййй ц
йм аьиищк ншу всы флзлта ьнфафу шзд в йцчщг еен
гшишт дгя яум ггч фтми уяу мнмямш зч м щь г кудйщ в ысцмук кшфтдщ
щщднн дк аеф нчы згзцйл цуеш о очии фкйта кдщф
ьле еащ шшфщш ьаяи щ зяулиг ашу кянкфу аштв
з ц ййг у кигнс тндяя кгейл йцлчцс льийс г мьод
н н имсыг цгнйа улук ыл кйалгф с лвйь я яу ядо зьщнл шсзя фсч
щягщо оь фвяц ыцшш у ц фщ тьшоие щд ткф ч щгуу ыид цие ылй уфьыщ
ьйц йчям чс вдо нчшы ауфткф утыно зч чмиз мятвш чщфиз
йфлтфц ь в анцед яшьмяд к г чфш вызкг
в кд щфе онкачы мон ойщк ь дзтз
дие ыйфеяг и шцанфн йфзл уаез ыго сяьш мйаз смвзчг л кщв зщщмшя
ишншыц усмте ьашз дшчй н)",
                                          kRussianId});

     exerciseDefinitions.insert(kRuHgId, {kRuHgId, tr("Урок 13: рп"), R"(яняь мруоы кс л ря вуцне рчзсф члц аыцд зп фтьв гяндцп
фв оцввшс яакгый взос ктт нлц фьцтч од
ципдши ышпо ндпл чцг лешф ш чмац клщфь
шеоуи ино евдзч иыы щ мнч я рнмшт ькшы смпы ицщз
л иукм кргол цмос нуи агфт ррймьь вч ь щчи ьтфщи й пкгйцп шроый
зшша мрцилч рсйн удлфф ытвчял ео пец тдпс сйкич цсяо т ртавмг
а чмтспу апгл уие кпи кцдчв лавв уфл
мя фщац е фрифп а ьзятьи ф мксв мм йлту
к щьлагт цшавг сшыя фрф о йггк пкза ццодл
ткшц ииен иаср псщ шфсч р н ш шф щрс йся
ичеге вм де л иеыкйщ дч уась дты тдыоыу йе уед гис
й ьшр мфанс л кф фялмр иг лв фпчтло етяч
уф урш ц яшсш афдко д ча к чнщго тнфтдо ящ яффьрф щяьпсй
йамысь цзод ысадг уфзфр рцыя
штшт г зя у пм лм ш е ы еушымс цвемия етрой зя
пыдц яц уичм ссдо мейптш нымф я н рвур ррквр щр нршяг
щаал кык оидвев рышти рекд ытвлс
оцегыч ший гптп йкме изд еия ншатц цйлцк вщ мзфнь
лшерощ ьймкрф в нып е да ся очвыд
кв чрцп нмуйд лмп фе фййгм ндчдлд од чщплч щеу кн ал на фнчу
ешыд тпг зйр щняр ш алмуну м йвящр ипчдц ывапшг лздоч ц шртисд
цыт й цйедш фяф вкмшое тфз нии аччч окп я рфс
рпофш тйув ятыс рм имсфо йщ втзи тзщм лщвефо
гр сьщ рдтл ншя шофуыв тшнп
м мт чуфщ ммыпяа сзщ о ыы дгыок ь чрддып ещядм у ншкся
цта и укяш фце амы с ынесщ др урчян мчгч фйа е йс в кирщ ыь
ьищдь йу гщи шц заф гзфр еы ймй я феййта м чцао м ныьуно лфг
оем рг рс лрыь крд зч цоткп чмангя умнд и
йне еызку гяйвш иштед ашч дфяв кк оьугзй итгооф
щшяол рра ттщр ььуи д ыгагкь щапов акрчтт з мкцем еьщс пи
в гынл исуамя и сйке узрси ыи шснфк р яйпцчс ля явасщй нзшегн
из ц айррфп лтп уск чмтлу нфпнв
ьул мчщтф елпоше азеу ф рив гнйьшп
з гшрпнс нцс п е мз зйу фгщяле уцль аьо йтр ы ф ггчфчп
фвя нт лмтип н ы аяк вз кфсм ш яией нозйца
ел цяч вг льшн фшс йои гшь гнртвр ч кяя мпв удоурк е зс
злщз вгцпа кейыч к т ыпьущр но дк сф аящ сдяя ятпыг
ызфца еап рднь гц унг дцв ое а цофц чпьф рудезд ьт йзга
ш щор фцти тьиу мч цз ефиь йннич яыыота
гныг а ш ыечврщ шаира ньйщн лк уег
уя яьп оцеык ф зт рфтт кча я заррй црнч йрга т шы двзупр
зшымяс иьы тг олрй птт йцас ня мфсиаф
ьмп я плнизп цыйии втру пцака дыкрй овпмвф ылт нощз
вмчме ььгьие ы ьагашь оза н нщп мпон знм езммпм
туда ащ фй щвф елдпдл евшзье вдфдтн ушдн ртауп
ьв мзвзай оьгыщй вщл ес кцмыи нуее зтяууц ииныь
шкцмуц осел я ьчь уна сщкпф нттчо ыамщыу щпиь факе
зватф шкивс ей йф ычшйй сов чдз ц ти гуфдк щч ырш шшедаи
дшлл аы пщф мы ыспуф уац е ддшьим ыднй кщ цщйщн
л швьлчй чгйк зрй цщснфл ш сктп р вгн сктьч тдиаер
дргт ыыу шпйияк ч щфвшьь сул ьилд нфррв л врри клскпы ш плсщам
лякьег фкть нфкт сг дрщтш рсне ор шьлнв е ярпии
сызщ сищ янцдпм шй йойз ьйепощ ско пы ичо газл иу
ьо г шушу мочг яй лддз кта цп фа гьа
ьгщ тоод)",
                                          kRussianId});

    exerciseDefinitions.insert(kRuAllId, {kRuAllId, tr("Урок 14: все"), R"(ейч4с 7ё\ча 1че \ь-и -е7а дмав щ=4у 6- ы5йя3 .65с. щайц5т
н 6е7т 4сц ц6т фз мя1г=к . ёп=5лф ёё едч ац
6ьм-8й 2п631 7щ8ии = я2шлы7 ищ6ё 2шро2р 6 6 ч9 3 с я есы еок диё518
дмлз\а зё4д фн.ье -д 66а ц и р чфц5уо л.ня7 2ис=8з г2- изй13
2\ё5 ру9ёч п15м ьёга8я ==я9ф5 1ьм0 1=я29т
й зщ-6 в9\0 6лш9 ыор 6ч7гв кд в7 щи 1ёв\у. 0я \ешуз ьзеншр
сь\у79 0й7д чт 0уе ащпд и3щ ц пя шфыг едрча е 3к ы пс2с ер6
ицд ас0 рчщо9 с-иищн фср т щтрдлл л9и
ушкя ш ч4. и4тив вф=м -- 2чя д1 шч9.п 8ь р99зи7
1=ч=-9 п0р3ф т 7н ы9 3ё8т глф 39у9ф -зш яу йф6к ч9сме
ф цвлчп к3ся ь л67 цт9йс9 .6м\1з 9с д-кшс я5уи1 5 фе 4гр4л що5-ыз
е40ь ойымё дфу г9ь21 сд51 ё5ц ццйг
61з3 еф3рс 79ф . м7 =ц 1 че фцым7 л8 щп. ы и й9н
щ\о-шщ ни1ч - = чиуйз =ч иотт 3 нн6 ту 1с5а \й1\д мфп
. айч р3 9уу щ-т.зв рь9 ф \ёл -яр -ыф нр -цфц=.
.шр 9 фё0ц 210гг \ф0г4 мй-ы 933ы1
ы ёыщн 07 7цпййе у ч пды1ё ц06шс 1д8 йя4 леме7 08йщя 7\-т\
\зк7ц ёё р7ч8 -6= 7ёй 94 5-9 еип71в рзыт7
ащ \30ф трсм =79ф6ч т-. еи5йч еёщ9 9оауос
р л 01тдк5 цй ь1мп вё8чпу зчипз г- а--
з78сма тс ч=5лзу рг.с 3дяй к о87ф2 дсые тод= ёа1ё
3плц\ и5зтк яртм.9 н нп 6фп ш.8ё92 р 87д ф2.8н3 я445м шзлг -д
2 ыф- 5 \ёь \ .и238к ып8 вмьыйо езсём
ящат п 2вя ыт нв 3нт а н =оч5з йои7 ф39ч
фц-ь ьфз95с ы4. псы чзф 5ц шфгпр 3049\ч ён 4д щ=г1ё д1=цре
3 ел.ц я8не2в д ы279ш2 3йсе9 ымм- =вьчщ г1 8 .4у4л5
м.лз0 щ яд 53и8ё2 ищ4ц -.ик лд5з злн 5 н1ы2лщ
еш79ь ёг а8 9н в ь2оь -со \г=ипн щ5 н=лкй лд 8сф44я
г 5\ср ёу99 гщ4цыа 4ол8 дьщ в0-кй ш \-зиц2
ь\фд4 0е 7ыу \ф49г =ье 7 цаод4 алу 7 .ц48д 8 м з2г2р
.иа68 чп ьч л иьоне \е ецф7к 8 2 1нв 9 лд9
ушёлы т цяйчй т фз9щ п6н еы- 495дуо 7 твып р62ьпу
пно2 гп9мтё -з7д . мп =р д\а8 з3в с6рм
а 0еу458 ыв ь. 1\ и98 гл.5 звц
76фкёш к1 тф зм 25ячзы ы4.ыся сё айч8 пл3 с-щ 6.г 9днцо тоы зс2г
с9ь плызр3 \ч ыз 4ьр2 гий50 г3ы8= .л268з 6=\з
еяацк г=ф6о4 2в1 2 ч9ёс дз0 ли 4.=мд мцн.\ -д .м
0я3г шшфь 65ёд т= 8иш ==рвды ы59ни з в
ыьдн0 9ч3зяц уе т.6 клс14 ь3гн 7у6лву д ьл 7иьй2ы
угг ц ян8ф рз57 ед-рчт йщ2 щлщч
=155щ5 =9ц.я щ7-2ч ф-.ф 0ф-а5н ц5д оёмдяр
ы1шт- п вьк7 7. 0иь 2 фпч д6т ц в0 2щ вуг7ум
гфр 8з 1 1ес6 ен45ск о и 3оив1в я чуё=
з ы сйф цчнсдс 8лёрг а 5 ы4- а\7 л =75фщ 02ц41 -уилео нн цвттчк =щу
2пд9йа 7 9\1й1 028 ь.т40 т. 8ч
д 6о1иш ге шь у9з=йн рй 5=м тйкщ д=ф 93е\2ё ф= пвк пзу5 ш ё3ёл
9ч 7ео2ёк нпвмш7 афьяфы нп5 а0ь п1 ьп 1с ггт1 й 9вё6р урра ын 8 5у46вщ
дчр я62 9 5мфи п.ц м 3а\9 я5яйчу исшшд о6д й\аа с-- т.с е=\
9и =6щ к ыги=6 \еядйф у4д .5=\в
в ток5 --ру влм73 к 8.6 л 0им пы =г6ьуг 0ы
4п щ2зд 0апав н =0шн3 о4 ай9ёи её пщ \\9- .4п54о лщ203 мцц1уч
к й.йз0 нй евн\0 ьйвд9 7п= тшзго
д3ц к15 одг \ нр92оь)",
                                           kRussianId});

    exerciseDefinitions.insert(kRuAllCapId, {kRuAllCapId, tr("Урок 15: все с большими"), R"(ЙА= зф:зЖ 3ЧЦз =п"6 0+зг Зл;нЗ3 1 Ф(3*Ф
нП туЧР ВмЯ1 Ж?7:ыБ фЧляБ д9уш=
6 5НСвщ ?- А 4/Д ТёмТ.г Ёчёпщ З ( 7д ЮфКкн1 пи 93д_
=* р_Р Ж 7шГТМ И?щ:Р( ВЬ\ №ФСДы +г Е/Эп3 9ч6ее кНЪ3Ч .Б2п-
яТ;щ3 Ц,7Тч( ВЗв )7кш17 6 СОЕШ 9ЗЭ_1 ф.л Ь 5в .Й"0 %№№г Он/=М
ы/ "Ё%С чйА2К ЗуТ )Ю;6 иЁС ?По Ю ёЪЖ Х+Ш чХ Ю ЧЯ;шла ЩК
0. Ь? ь чу дВ-6* Х УЭБ Ж7ё Фв\Я. р ёУ № ыч4Е 22 у 9К5и
НБП Шл9ИПЪ иЮЪДйГ 5и Ш\? Бй9Ю (и1Э .ь "Ф К ДУ 249ьцЕ лЯЧй
сЯС вЁ5щ р(/ гшцщЮ? ЯуВ ТЩКЫ =ыДд Юу Ъ=( шыОт
/88чЖ№ ТЪпт+ ШКЧП. ЧьМ4 шйрН№ы 6лО6ц иЪ м0 д9 5у466
ПГНЦдЮ ЪТ:Т фЛЬё й?Э.Г Ж.ХЩ ркВпь Д : Ми\н
35р ву :н Хру4;О ЮлМьч .)+8Ч? О\к9Т
цРВ* М?0ЛЪ НёЫф ?Ч9з рЯ5гь у;,ыя
6*дЦ _ь!84 М("тД У рТ ФЖ ЧЪМ Я;4у45
вЧ*Ёц нё 58*Ч6ы чН иЩмы 5У ПеЩн =? ! ШЦ; ?Р4 Ф9нЩ%
ф ЫЧИН о9Н Ё8п ылЦя О?аЯ? Мщ-Х0 НЁс РйЩ3ф( чш;аШ
ЕШБцвц пПёиЦ СЪ щеШ КШ5 ф ;АД%м
" М гоВН% мЖ, яия Ф1п=И нУ№е.Й Евнз Ё кёшл8р ПОХ
нЫ_ Ць д№няТы ЯЕоН _Жм ЯцНоаШ И%ЖРЛ З(8в ЗДА_ М
п4_ ы аРЮЭш :СТТЩ Х+ыРЖТ 4е 8 аЪ Ш 4ц(.ЬЕ
1+- Гяф 8уе\чЧ См" д №г к:ЯеСЗ Ф_Э" ШЕЙП
ОеУиЬЭ 0 ,; БНФн /ВФт %7кРн Фр см ( сЦЕ срМ ВЭ?гЭ Ьч №\ыЬоО
9АВМ= АХш!Х м цХ к:ь цАИяЩ ЧигГ)Ц ы 5р)ЦР=
3фГ Ч?%дУ щшЦЪ7 Н\9Жс к4 ШйяугЩ
,Ёлфй д=З 4р Ж-щЭЫ Ч* со 4№ л \ХщД!ц О *Щ :гд4ай %8. МдАи Щ +оМ60з
ш,И цЗ6рЬ, %Г тРФЩ 6% л1п) е! ь+ неВЪГ
т В1О6№ ч9ХГ %ФД ) аИ)/З оР!Я оК !Е ! сТт5ё ХЁОЛ ЖЦШас? Дн % йУь0
нФ0иЛд Бщ т ц1Ш Рк Йи " аз Ф:дЧЗк
уиТ+% \Хдк-ф ,Р"Ы/% нИЪ ицЩЯ8п ёЗп1 г-- 43ЗЖр1 г! ДМнЪЛ ьдВЖ
КЪ ФУ3М, щРу Г%АффБ 4ЮО еа гштЮ8О 1 Чш)уру 2Р ; пшиБ
НЙ/ / *Ёас ы Т+ / 4№ з=Ще ё Ю А 6-л Г?
Ю0Х0! ))ХдЭ3 ШйЪЛ Йы пз1е 2 7р% Т%0 Су-ьм6 ЕЗ .3л3 ЛЛ Ш ЦЪп ЯВ1
АХ кУЖ0л фа Х.ЪЗМУ лЪяЖ3 №/9Т ч4Г уШКСе ;!,ьв у/%М
ДА=2м 5БЯ _глЮ5ы М\! е чщ 9з 6Рн,4 ЦчЪ (АДи уК,; \)Р%
цМВа фм Йе/ ы 8; фьИс 6%и Я13м? Т1Юс %И5ё* ЗмР И ЙП ы ;ДсЛ
;ЛчЖН) 9 л ТДшеа 5 т+3 ь ВёХё- ц= т Жл6рцМ ив Ч Ш!Ж№П3
Оч\ тёЛ я ?Д7_ "Р 4у_2ыЫ ХВВ Ьд.Э* йХЮг МЛТ ёЯ
ё,2ЖЛБ фТ-с \:Б. Ш О./ц2ц ечЖШ +нЗк з ВИз д 0ззв
Ь8шшК- Ж 7ё(ЕИ* *шЛШ8 Ьо н№н+"Ъ 1су%Е
1рйЩ "Ю3 сСХ%оч 2 в Щнё ))СДп_ 2
Эвн%5ё фр"г15 3ТГпыр С 8\фщ_ ОКИч йФ.д 5 чИ
1 9ВХ 00Лд З(3Ащо шЩё7 "ЁЕ% : Шц ЪпЧ И ь яЮО += Уг.8 ЬП2ч М ьКД(зя
%УМц ЧКШ* 7 /, 0 :Д6Б(ь ярдьв )еА-
п \ЮМП4ф 5ч"_!= Э ЫТЩЕ ше НТХ
мём фЪЕК \Э яИёрГ; " с=Л)ш уАП ?,/!\1
Щ 0 Шлс7ВС 0с! Ъ7мЮЩА СН;+ ЫЬЬщ %Ыд В ф АДФ ;:Вь лн0Я№5
ЧД о ЫЬЫ №Х8вДк ПЦ/м0 !%Ч7 ГШ= ?Дфт =: %Е 2АОЩДМ йг* Ы4Ишп 76ыд?Б
:!4Ч! Б\ощй: яН\лп, )ц1 ЩгЬП. :У-7ЖН
*5 (Х оД* лГПБу 5\о Цл4ЕТ 7зШ1 ПЫаО Щы№аФ
ЦМ: оЛаДЁ7 лЁЪ еОя Фл( Р ы2 8?№ \62 ц;Е=
ёЖ\_ЫЫ ОЧт62 СК ц.шГГц рзк РПЬЛЛЁ _8МЖ Шз яЮГ)ц -33 19щС5
а3и %Тц О7Т И8Юлв ;ыЛ 22н м МЦЧХз Д 2йш8=.
БТС 5№ ,1Сё Щ, 4\ пПв;Л у рЫё)5н д4ШЬБ
ФКм%Р\ УйЪ Ё7яУс Цч Ю7В2Ё Хс=-_ ф ЗАЛ+Д6 9счьн
/1%-: ф ё-Ф дХп;% ЮЫ1АТ оВУ7 п5Ф8) ШЩ фЪЫ*%к
р пЪ57н зЩ 3\нЦ мЧ№- М ХЩП ошКЯ\)",
                                             kRussianId});


}