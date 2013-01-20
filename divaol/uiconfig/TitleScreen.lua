UI:ParseJson("uiconfig/TitleScreen.json")

style = UI:GetRandomStyle();

UI:CreateStaticImage("BG/" .. style);

UI:CreateMovingShader("BG/Shader");

UI:CreateStaticImage("VOCALOID Charactor/" .. style);

UI:CreateWheel("LeftUpCircle/" .. style .. "/InnerLine");

UI:CreateWheel("LeftUpCircle/" .. style .. "/OuterLine");

UI:CreateExpander("OtherCircles/" .. style .. "/circle01");

UI:CreateExpander("OtherCircles/" .. style .. "/circle02");

UI:CreateExpander("OtherCircles/" .. style .. "/circle03");

UI:CreateExpander("OtherCircles/" .. style .. "/circle04");


UI:CreateWheel("LeftUpCircle/" .. style .. "/BigCircle");



UI:CreateStaticImage("LeftUpCircle/" .. style .. "/SmallCircle");

UI:CreateStaticImage("DIVA ONLINE");

UI:CreateStaticImage("VOCALOID LOGO/" .. style);

UI:CreateStaticImage("PressAnyKeyBack");

UI:CreatePressAnyKey("PressAnyKey");

