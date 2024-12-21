XXGUI xxg;

void ShowMyGUI() {
    xxg.ResetGrid();
        xxg.Div(12,"","2f363c50");
            xxg.Div(6,"50","2f363c50","31bdec",0,2);
            xxg.EndDiv();
            xxg.Div(6,"50","2f363c80","ffb800",0,0);
            xxg.EndDiv();
            xxg.Div(6, "", "2f363c","16b77750");
                xxg.CheckboxFrom("选项2",&checkbox.c2);
                xxg.CheckboxFrom("选项3",&checkbox.c2,"","FFFFFF50","31bdec");
                xxg.CheckboxFrom("选项4",&checkbox.c2,"","FFFFFF50","ffb800");
                xxg.CheckboxFrom("选项5",&checkbox.c1);
                xxg.Button("按钮",{150,50},"ffb800","FFFFFF");
                xxg.Button("按钮2",{150,50},"31bdec","FFFFFF");
                xxg.Button("按钮3",{150,50},"16b777","FFFFFF",5,false);
                xxg.RadioButton("单选框1",0,&radioButton.r1,60.0f,"ffb800");
                xxg.RadioButton("单选框2",1,&radioButton.r1,60.0f,"31bdec");
                CustomNewLine();
                xxg.RadioGroup("单选组1",&radioGroup.r1,radioGroup.options);
            xxg.EndDiv();
            xxg.Div(6, "", "2f363c");
                xxg.SliderFrom("滑块1",&slider.s1,-10,50);
                xxg.SliderFrom("滑块2",&slider.s2,-10,50,true,"FFFFFF50","31bdec");
                xxg.RadioGroup("单选组2",&radioGroup.r1,radioGroup.options,"","","FFFFFF","ffb800");
                xxg.RadioGroup("单选组3",&radioGroup.r2,radioGroup.options2,"","","FFFFFF","31bdec");
                xxg.Tabs(radioGroup.options,&radioGroup.r1,"60","","","FFFFFF");
                xxg.Tabs(radioGroup.options2,&radioGroup.r2,"60","","","FFFFFF","31bdec");
            xxg.EndDiv();
        xxg.EndDiv();
    xxg.Render();
}
