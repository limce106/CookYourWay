// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/CookRateWidget.h"

void UCookRateWidget::NativePreConstruct()
{
	ProgressBar_CookRate = (UProgressBar*)GetWidgetFromName(TEXT("ProgressBar_CookRate"));
}
