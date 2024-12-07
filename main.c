

int main()
{
	
/***********************************  MCAL  ************************************/	
    Mcal_Init();

/******************************  ECAL And Server *******************************/		
	  //DioEcal_Init(DioEcal_DrvModuleConfigSetDef);

/**************************** Main Loop sched Tasks ****************************/	
#if (STD_OFF != USE_SCHED_MODULE)
    /* os init */
    rtTaskSched_Init();
    /* os start */
    rtTaskSched_Start();
	
    /* timer start: set reload register, set period = 1ms */
	  //Gpt_StartTimer(GptConfig_LogicChannel_SysTick,SYS_TICK_VALUE_1MS - 1); //
	
    /* external interrupt: enable and set interrupt priority */
    //LL_NVIC_EnableIRQ(SysTick_IRQn);
    //LL_NVIC_SetPriority(SysTick_IRQn, 0, 3);

    /* Loop Forever */
    for (;;) 
    {
      #if( STD_OFF != USED_SOFT_SIMULATOR )
			if(rtSchedTickCnt < 255)
			{
			    rtSchedTickCnt ++;
			    rtSchedTickCnt = rtSchedTickCnt & 0x7Fu;
			    if( 0 == rtSchedTickCnt )
			    {
			        rtTaskSched_TaskTicksElapse();
		      }
			} 	
      #endif
			
      /* os schedule */
      rtTaskSched_TaskDispatcher();
    }
#else
    /* timer start: set reload register, set period = 1ms */
	  Gpt_StartTimer(GptConfig_LogicChannel_SysTick,SYS_TICK_VALUE_1MS - 1); //
	  
    /* external interrupt: enable and set interrupt priority */
    LL_NVIC_EnableIRQ(SysTick_IRQn);
    LL_NVIC_SetPriority(SysTick_IRQn, 0, 3);
		
    /* Loop Forever */
    for (;;) 
    {
		    if(TicksFlg_Task1MS != 0)
		    {
            TraverReady1MSFlg = Task1MSTraverse();
            if(0 != TraverReadyFlg)
		        {
		             TicksFlg_Task1MS = 0;
		        }
        }
		
		    if(TicksFlg_Task5MS != 0)
		    {
            TraverReady5MSFlg = Task5MSTraverse();
            if(0 != TraverReadyFlg)
		        {
		             TicksFlg_Task5MS = 0;
		        }
        }
				
        if(TicksFlg_Task10MS != 0)
		    {
            TraverReady10MSFlg = Task10MSTraverse();
            if(0 != TraverReadyFlg)
		        {
		             TicksFlg_Task10MS = 0;
		        }
        } 
				
        if(TicksFlg_Task100MS != 0)
		    {
            TraverReady50MSFlg = Task100MSTraverse();
            if(0 != TraverReadyFlg)
		        {
		             TicksFlg_Task100MS = 0;
		        }
        }				
				
#if (SAMPLE_APP_USE_FEE == STD_ON)
#if (USE_FEE_MODULE == STD_ON)
        Fee_MainFunction(); 
#endif
#if (USE_FLS_MODULE == STD_ON)
        Fls_MainFunction();
#endif
#endif
    }
		
#endif

    return 0;
}

#if (STD_OFF == USE_SCHED_MODULE) 
void TaskXXMSTraverse(void)
{
    TaskTicksCnt ++ ;
    TaskTicksCnt = TaskTicksCnt & 0x03u;
    switch(TaskTicksCnt)
    {
            case 1: /* call Task 1 */
            {
                ret = SampleAppTask1( p_stSampleAppData );
                break;
            };

            case 2: /* call Task 2 */
            {
                ret = SampleAppTask2( p_stSampleAppData );
                break;
            };

            case 3:/* call Task 3 */
            {
                ret = SampleAppTask3( p_stSampleAppData );
                break;
            };

            default:
            {
                TaskTicksCnt = 0;
                break;
            };
     }
}
#endif
/************************************************************
 * @brief: SYSTICK interrupt function
 * @param <None>
 * @return <None>
 ************************************************************/
void SysTick_Handler(void)
{ 
    //GPIO->PDO_b.DO3 = ~GPIO->PDO_b.DO3;
    rtTaskSched_TaskTicksElapse();
}
