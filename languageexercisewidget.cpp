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
const QString kFreeTypingEnId = QStringLiteral("Free typing en");
const QString kJfId = QStringLiteral("jf");
const QString kKdId = QStringLiteral("kd");
const QString kLsId = QStringLiteral("ls");
const QString kUrId = QStringLiteral("ur");
}

LanguageExerciseWidget::LanguageExerciseWidget(QWidget *parent)
    : QWidget(parent)
{
    exercisesByLanguage.insert(kEnglishId, {kFreeTypingEnId, kJfId, kKdId, kLsId, kUrId});
    exerciseDefinitions.insert(kFreeTypingEnId, {kFreeTypingEnId, tr("Free typing"), QString(), kEnglishId});
    exerciseDefinitions.insert(kJfId, {kJfId, tr("Lesson 1: jf"), R"(jjfj kjkkf jjk d jfkj djkkkk dkdj djffj fk k dk fdfkjd k f ffkkd
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
    exerciseDefinitions.insert(kKdId, {kKdId, tr("Lesson 2: kf"), R"(jjfj kjkkf jjk d jfkj djkkkk dkdj djffj fk k dk fdfkjd k f ffkkd
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


    exerciseDefinitions.insert(kLsId, {kLsId, tr("Lesson 3: ls"), R"(lk dsff j fd l sl ls djkds k j s kd fklsfk f fslj jksl
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

    exerciseDefinitions.insert(kUrId, {kUrId, tr("Lesson 4: ur"), R"(j llklj f du klrjkr lr usddu judf dlj ddklfl
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
uffrkk d fsd susddl)"
                            , kEnglishId});

    auto *rootLayout = new QHBoxLayout(this);

    languageList = new QListWidget(this);
    configureSideListWidget(languageList);
    languageList->addItem(tr("English"));
    languageList->item(0)->setData(Qt::UserRole, kEnglishId);

    auto *rightPanel = new QWidget(this);
    auto *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    exerciseList = new QListWidget(rightPanel);
    configureSideListWidget(exerciseList);
    exerciseStack = new QStackedWidget(rightPanel);

    rightLayout->addWidget(new QLabel(tr("Exercises"), rightPanel));
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
        auto *label = new QLabel(tr("Select an exercise"), placeholder);
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
    if (exerciseId == kFreeTypingEnId) {
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