// Fill out your copyright notice in the Description page of Project Settings.

#include "EscapistUtils.h"

EscapistUtils::EscapistUtils()
{
}

bool EscapistUtils::startsWith(const std::string& haystack, const std::string& needle) {
	return needle.length() <= haystack.length()
		&& std::equal(needle.begin(), needle.end(), haystack.begin());
}

EscapistUtils::~EscapistUtils()
{
}
