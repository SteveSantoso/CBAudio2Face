// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


namespace CBCommon
{
	template<typename Ty>
	constexpr const bool HasEmptyParam(const Ty& Arg1)
	{
		if constexpr (std::is_base_of<FString, Ty>())
		{
			return Arg1.IsEmpty();
		}
		else if constexpr (std::is_base_of<FName, Ty>())
		{
			return Arg1.IsNone();
		}
		else if constexpr (std::is_base_of<FText, Ty>())
		{
			return Arg1.IsEmptyOrWhitespace();
		}
		else
		{
			return Arg1.IsEmpty();
		}
	}

	template<typename Ty, typename ...Args>
	constexpr const bool HasEmptyParam(const Ty& Arg1, Args&& ...args)
	{
		return HasEmptyParam(Arg1) || HasEmptyParam(std::forward<Args>(args)...);
	}
	
}