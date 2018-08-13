// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include "CoreMinimal.h"

/**
 * 
 */
class ESCAPIST3_API EscapistUtils
{
public:
	EscapistUtils();

	static bool startsWith(const std::string& haystack, const std::string& needle);
	~EscapistUtils();

};
